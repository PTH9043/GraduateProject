#include "EngineDefines.h"
#include "UMirror.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "UShader.h"
#include "UVIBufferRect.h"
#include "URenderTargetGroup.h"
#include "UTransform.h"
#include "UScreenRenderObj.h"
#include "UMirrorCamera.h"

UMirror::UMirror(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON),
	m_iRegisterCamID{ 0 },
	m_spVIBufferRect{nullptr}
{
}

UMirror::UMirror(const UMirror& _rhs) : 
	UPawn(_rhs), m_iRegisterCamID{ 0 },
	m_spVIBufferRect{ nullptr }
{
}

void UMirror::Free()
{
}

HRESULT UMirror::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UMirror::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	AddShader(PROTO_RES_MIRRORSHADER);
	m_spVIBufferRect = AddResource<UVIBufferRect>(PROTO_RES_VIBUFFERRECT, RES_VIBUFFER);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	// ScreenObj 
	{
		UScreenRenderObj::SCREENRENDEROBJDESC tDesc{ ThisShared<UPawn>() };
		m_spScreenRenderObj = static_pointer_cast<UScreenRenderObj>(spGameInstance->CloneActorAdd(
			PROTO_ACTOR_SCREENRENDEROBJ, { &tDesc }));
	}
	// MirrorCamera
	{
		UCamera::CAMDESC tDesc;
		tDesc.stCamProj = UCamera::CAMPROJ(UCamera::PROJECTION_TYPE::PERSPECTIVE, _float3(0.f, 0.f, 0.f),
			_float3(0.f, 0.f, 1.f),
			DirectX::XMConvertToRadians(60.0f), (_float)(spGameInstance->GetGraphicDesc()->iWinCX), 
			(_float)(spGameInstance->GetGraphicDesc()->iWinCY), 0.2f, 10000.f);
		tDesc.stCamValue = UCamera::CAMVALUE(5.f, DirectX::XMConvertToRadians(90.f));
		tDesc.eCamType = CAMERATYPE::OTEHR;

		m_spMirrorCamera = static_pointer_cast<UMirrorCamera>(spGameInstance->CloneActorAdd(
			PROTO_RES_MIRRORCAMERA, { &tDesc }));
		m_iRegisterCamID = m_spMirrorCamera->GetCamID();
		m_spScreenRenderObj->SetRegisterCamID(m_spMirrorCamera->GetCamID());
	}


	return S_OK;
}

void UMirror::TickActive(const _double& _dTimeDelta)
{
	m_spMirrorCamera->GetTransform()->SetPos(GetTransform()->GetPos());
	m_spMirrorCamera->GetTransform()->LookAt(GetTransform()->GetPos() - GetTransform()->GetLook() * 10);
}

void UMirror::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
}

HRESULT UMirror::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);
	GetTransform()->BindTransformData(GetShader());
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0, m_spScreenRenderObj->GetScreenRTGroup()->GetRenderTargetTexture(0));
	m_spVIBufferRect->Render(GetShader(), _spCommand);
	return S_OK;
}

void UMirror::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
