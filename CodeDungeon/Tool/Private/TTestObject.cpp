#include "ToolDefines.h"
#include "TTestObject.h"
#include "UGameInstance.h"
#include "UVIBufferRect.h"
#include "UVIBufferGrid.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UVIBufferPlane.h"
#include "UShader.h"
#include "UParticle.h"

TTestObject::TTestObject(CSHPTRREF<UDevice> _spDevice, 
	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::STATIC),
	m_spVIBufferGrid{ nullptr }, m_spRectColorBuffer{nullptr}, m_spParticle{nullptr}
{
}

TTestObject::TTestObject(const TTestObject& _rhs) : 
	UPawn(_rhs), m_spVIBufferGrid{ nullptr }
{
}

void TTestObject::Free()
{
}

HRESULT TTestObject::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TTestObject::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spVIBufferGrid = static_pointer_cast<UVIBufferGrid>(spGameInstance->CloneResource(PROTO_RES_VIBUFFERGRID));
	// Add Shader 
	AddShader(PROTO_RES_GRIDSHADER);

	m_spRectColorBuffer = CreateNative< UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::B3, RECTCOLOR_SIZE, 1);
	m_RectColor.g_RectColor = { 0.f, 1.f, 0.f, 1.f };

	//{
	//	UParticle::PARTICLEDESC tDesc;
	//	tDesc.wstrParticleComputeShader = PROTO_RES_COMPUTEPARTICLE2DSHADER;
	//	tDesc.wstrParticleShader = PROTO_RES_PARTICLE2DSHADER;
	//	tDesc.wstrParticleTextureName = L"Flare_Sparks_TexS";

	//	tDesc.ParticleParam.stGlobalParticleInfo.fAccTime = 2.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fDeltaTime = 2.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fEndScaleParticle = 5.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fMaxLifeTime = 10.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fMaxSpeed = 5000.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fMinSpeed = 2000.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.fStartScaleParticle = 10.f;
	//	tDesc.ParticleParam.stGlobalParticleInfo.iMaxCount = 300;

	//	m_spParticle = std::static_pointer_cast<UParticle>(spGameInstance->CloneActorAdd(PROTO_ACTOR_PARTICLE,
	//		{ &tDesc}));
	//	//m_spParticle->SetActive(false);
	//}
	return S_OK;
}

void TTestObject::TickActive(const _double& _dTimeDelta)
{

}

void TTestObject::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
}

HRESULT TTestObject::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindCBVBuffer(m_spRectColorBuffer, &m_RectColor, RECTCOLOR_SIZE);
	m_spVIBufferGrid->Render(GetShader(), _spCommand);
	/*m_spParticle->GetTransform()->SetPos(GetTransform()->GetPos() + _float3{ 0.f, 50.f, 0.f });
	m_spParticle->SetActive(true);*/
	return S_OK;
}

void TTestObject::Collision(CSHPTRREF<UPawn> _pEnemy)
{

}