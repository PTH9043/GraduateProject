#include "EngineDefine.h"
#include "UTrail.h"
#include "UGameInstance.h"
#include "UVIBufferTrail.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UCollider.h"
#include "UMethod.h"
#include "UTexGroup.h"
#include "UTexture.h"

#define BLUE_COLOR4 1.f / 255.f, 165.f / 255.f, 172.f / 255.f, 0.f
//#define BLUE_COLOR4 1.f / 255.f, 102.f / 255.f, 200.f / 255.f, 0.f

#define GREEN_COLOR4 65.f / 255.f, 150.f / 255.f, 43.f / 255.f, 0.f

UTrail::UTrail(CSHPTRREF<UDevice> _spDevice,
	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON),
	m_spVIBufferTrail{ nullptr }, m_spTrailColorBuffer{nullptr}, m_vCurTrailColor{_float4(BLUE_COLOR4)}, m_bRender{true}
{
}

UTrail::UTrail(const UTrail& _rhs) :
	UPawn(_rhs), m_spVIBufferTrail{ nullptr }, m_spTrailColorBuffer{nullptr},  m_vCurTrailColor{ _float4(BLUE_COLOR4) }, m_bRender{ true }
{
}

void UTrail::Free()
{
}

void UTrail::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

HRESULT UTrail::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UTrail::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spVIBufferTrail = static_pointer_cast<UVIBufferTrail>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERTRAIL));
	
	m_spTrailColorBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::TRAILCOLOR, static_cast<_int>(sizeof(_float4)));

	if (m_spTrailTexGroup == nullptr)m_spTrailTexGroup = static_pointer_cast<UTexGroup>(spGameInstance->CloneResource(PROTO_RES_TRAILTEXTUREGROUP,_vecDatas));

	
	// Add Shader 
	AddShader(PROTO_RES_TRAILSHADER);

	m_fCreateTime = 0.0001f;
	m_iMaxCount = 12;
	m_iDivide = 8;
	m_fTime = m_fCreateTime + 1.f;
	return S_OK;
}

void UTrail::Update(const _double& _dTimeDelta)
{
	if (!m_bRender)
		return;

	m_fTime -= static_cast<_float>(_dTimeDelta);
}
void UTrail::TickActive(const _double& _dTimeDelta)
{
	Update(_dTimeDelta);

}

void UTrail::AddTrail(_float3 _top, _float3 _bottom) {
	if (m_fTime < 0.f)
	{
		m_fTime = m_fCreateTime;

		m_listPos.emplace_back(std::make_pair(_top, _bottom));

	
		size_t iCount = m_listPos.size();
		if (iCount >= m_iMaxCount)
		{
			for (int i = 0; i < 2; ++i)
				m_listPos.pop_front();

			for (int i = 0; i < 2 * m_iDivide; ++i)
				m_listRomPos.pop_front();
		}

		
		iCount = m_listPos.size();
		if (iCount <= 3)
		{
			
			m_listRomPos.emplace_back(std::make_pair(_top, _bottom));
			return;
		}

		auto iter = m_listPos.end();
		_float3 xmf3TopPos[4], xmf3BottomPos[4];
	
		for (int i = 3; i > -1; --i)
		{
			xmf3TopPos[i] = (*(--iter)).first;
			xmf3BottomPos[i] = (*(iter)).second;
		}

		for (int i = 0; i < m_iDivide; ++i)
		{
			_float t = (i + 1) / (_float)m_iDivide;
			_float3 xmf3RomTopPos = DirectX::XMVectorCatmullRom(XMLoadFloat3(&xmf3TopPos[1]), XMLoadFloat3(&xmf3TopPos[2]),
				XMLoadFloat3(&xmf3TopPos[3]), XMLoadFloat3(&xmf3TopPos[3]), t);
				
				
			_float3 xmf3RomBottomPos = DirectX::XMVectorCatmullRom(XMLoadFloat3(&xmf3BottomPos[1]), XMLoadFloat3(&xmf3BottomPos[2]),
				XMLoadFloat3(&xmf3BottomPos[3]), XMLoadFloat3(&xmf3BottomPos[3]), t);


			m_listRomPos.emplace_back(std::make_pair(xmf3RomTopPos, xmf3RomBottomPos));
		}

		
	}
}

void UTrail::SetRenderingTrail(_bool isOn)
{
	m_bRender = isOn;

	if (!m_bRender)
	{
		m_listPos.clear();
		m_listRomPos.clear();
		m_fTime = m_fCreateTime;
	}
}

void UTrail::SetColor(_float4 _col)
{
	m_vCurTrailColor = _col;
}

void UTrail::LateTickActive(const _double& _dTimeDelta)
{

	AddRenderGroup(RENDERID::RI_NONALPHA_LAST);

}

HRESULT UTrail::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	GetShader()->BindCBVBuffer(m_spTrailColorBuffer, &m_vCurTrailColor, static_cast<_int>(sizeof(_float4)));
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0, m_spTrailTexGroup->GetTexture(L"Trail"));
	GetShader()->BindSRVBuffer(SRV_REGISTER::T1, m_spTrailTexGroup->GetTexture(L"Dissolve"));


	if (m_bRender) {
		size_t iCount = m_listRomPos.size();
		if (iCount <= 1)
			return S_OK;

		auto iter = m_listRomPos.begin();

		size_t iRectCount = iCount - 1;
		size_t iVertexCount = iRectCount * 6;//사각형 당 정점 6개

		VECTOR<VTXDEFAULT>	Vertices;
		Vertices.resize(iVertexCount);


		int i = 0, iLineIndex = 0; //사각형의 왼쪽
		while (iter != m_listRomPos.end())
		{
			_float3 xmf3Pos[4];
			xmf3Pos[0] = (*(iter)).first; //Top1
			xmf3Pos[1] = (*(iter++)).second; //Bottom1

			//사각형 더 못그리면
			if (iter == m_listRomPos.end())
				break;

			xmf3Pos[2] = (*(iter)).second; //Bottom2
			xmf3Pos[3] = (*(iter)).first; //Top2

			_int iNextIineIndex = iLineIndex + 1; //사각형의 오른쪽
			_float2 xmf2UV[4];
			_float fRatio = (_float)iLineIndex / iRectCount;
			_float fNextRatio = (_float)iNextIineIndex / iRectCount;
			xmf2UV[0] = { fRatio,			0.f };
			//xmf2UV[1] = { fRatio,			1.f };
			//xmf2UV[2] = { fNextRatio / 1.f, 1.f };
			xmf2UV[1] = { fRatio,		(fRatio / 1.f) };//(fRatio / 1.f)
			xmf2UV[2] = { fNextRatio / 1.f, (fNextRatio / 1.f) };// (fNextRatio / 1.f)
			xmf2UV[3] = { fNextRatio / 1.f, 0.f };

			Vertices[i++] = VTXDEFAULT(xmf3Pos[2], xmf2UV[2]);	//xmf3Bottom2,
			Vertices[i++] = VTXDEFAULT(xmf3Pos[3], xmf2UV[3]);	//xmf3Top2,	
			Vertices[i++] = VTXDEFAULT(xmf3Pos[0], xmf2UV[0]);	//xmf3Top1,	
			Vertices[i++] = VTXDEFAULT(xmf3Pos[0], xmf2UV[0]);	//xmf3Top1,	
			Vertices[i++] = VTXDEFAULT(xmf3Pos[1], xmf2UV[1]);	//xmf3Bottom1,
			Vertices[i++] = VTXDEFAULT(xmf3Pos[2], xmf2UV[2]);	//xmf3Bottom2,

			iLineIndex++;
		}

		m_spVIBufferTrail->SetVertices(Vertices, iVertexCount);
		m_spVIBufferTrail->Render(GetShader(), _spCommand, iVertexCount);
	}
	

	

	return S_OK;
}

HRESULT UTrail::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderShadowActive(_spCommand, _spTableDescriptor);

	return S_OK;
}

