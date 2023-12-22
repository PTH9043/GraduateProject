#include "ClientDefine.h"
#include "CPlayer.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UTerrain.h"
#include "UTransform.h"
#include "UShader.h"
#include "UVIBufferSphere.h"
#include "CMainCamera.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "CMonster.h"
#include "CBullet.h"

CPlayer::CPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_spVIBufferSphere{ nullptr },
	m_spTexGroup{ nullptr },
	m_spTerrain{ nullptr },
	m_spMainCamera{nullptr}
{
}

CPlayer::CPlayer(const CPlayer& _rhs) : 
	UCharacter(_rhs),
	m_spVIBufferSphere{ nullptr },
	m_spTexGroup{ nullptr },
	m_spTerrain{ nullptr },
	m_spMainCamera{ nullptr }
{
}

void CPlayer::Free()
{
}

HRESULT CPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CPlayer::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);
	m_spTerrain = UMethod::ConvertTemplate_Index<SHPTR<UTerrain>>(_vecDatas, 0);
	m_spMainCamera = UMethod::ConvertTemplate_Index < SHPTR<CMainCamera>>(_vecDatas, 1);

	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	m_spVIBufferSphere = AddResource<UVIBufferSphere>(PROTO_RES_VIBUFFERNORMALSPHERE, RES_VIBUFFER);
	AddShader(PROTO_RES_NORMALOBJECTSHADER);
	m_spTexGroup = AddResource<UTexGroup>(PROTO_RES_CUBETEXTUREGROUP, RES_TEXGROUP);

	GetTransform()->SetPos(_float3{ 1, 1, 1 });

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	UCollider::COLLIDERDESC tDesc;
	{
		tDesc.vScale = _float3{ 10.f, 50.f, 10.f };
		tDesc.vTranslation = _float3{ 0.f, 50.f, 0.f };
	}
	m_spCollider = AddComponent<UCollider>(PROTO_COMP_OBBCOLLIDER, L"OBBCollider", { &tDesc });
	{
		tDesc.vScale = _float3{ 50.f, 50.f, 50.f };
		tDesc.vTranslation = _float3{ 0.f, 50.f, 0.f };
	}
	m_spFirstCheckCollider 	= AddComponent<UCollider>(PROTO_COMP_SPHERECOLLIDER, L"SphereCollider", { &tDesc });
	return S_OK;
}

void CPlayer::CollisionOthers(CSHPTRREF<CMonster> _spMonster)
{
	RETURN_CHECK(nullptr == _spMonster, ;);

	if (true == _spMonster->GetFirstCheckCollider()->IsCollision(m_spFirstCheckCollider))
	{
		if (true == _spMonster->GetCollider()->IsCollision(m_spCollider))
		{
			SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
			spGameInstance->SetGamming(false);
			SetActive(false);
			return;
		}
	}
}

void CPlayer::TickActive(const _double& _dTimeDelta)
{
	SHPTR<UGameInstance> pGameInstance = GET_INSTANCE(UGameInstance);
	if (pGameInstance->GetDIKeyDown(DIK_LCONTROL))
	{
		m_spMainCamera->SetMoveState(!m_spMainCamera->IsMoveState());
	}

	if (false == m_spMainCamera->IsMoveState())
	{
		if (pGameInstance->GetDIKeyPressing(DIK_W))
			GetTransform()->MoveForward(_dTimeDelta, 100.f);
		if (pGameInstance->GetDIKeyPressing(DIK_A))
			GetTransform()->MoveLeft(_dTimeDelta, 100.f);
		if (pGameInstance->GetDIKeyPressing(DIK_D))
			GetTransform()->MoveRight(_dTimeDelta, 100.f);
		if (pGameInstance->GetDIKeyPressing(DIK_S))
			GetTransform()->MoveBack(_dTimeDelta, 100.f);

		_long		MouseMove = 0;

		if (MouseMove = pGameInstance->GetDIMMoveState(DIMOUSEMOVE::DIMM_X))
			GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), DirectX::XMConvertToRadians(90.f) * MouseMove * 2.f, _dTimeDelta);
	}

	if (nullptr != m_spTerrain)
	{
		_float3 vPosition = GetTransform()->GetPos();
		m_spTerrain->TerrainHeightPos(vPosition);
		//	vPosition.y += 10.f;
		GetTransform()->SetPos(vPosition);
	}
}

void CPlayer::LateTickActive(const _double& _dTimeDelta)
{
	if (false == m_spMainCamera->IsMoveState())
	{
		_float3 vPosition = DirectX::XMVector3Transform(_float3(0.f, 10.f, -10.f), GetTransform()->GetWorldMatrix());
		m_spMainCamera->GetTransform()->SetPos(vPosition);
		m_spMainCamera->GetTransform()->LookAt(GetTransform()->GetPos());

		m_spCollider->SetTransform(GetTransform());
		m_spFirstCheckCollider->SetTransform(GetTransform());
	}

	if (nullptr != m_spVIBufferSphere)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}

#ifdef _USE_DEBUGGING
//	m_spCollider->AddRenderer(RI_NONALPHA_LAST);
//	m_spFirstCheckCollider->AddRenderer(RI_NONALPHA_LAST);
#endif 
}

HRESULT CPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0 ,spGameInstance->GetDefaultBackTexture());
	m_spVIBufferSphere->Render(GetShader(), _spCommand);
	return S_OK;
}

void CPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	
}