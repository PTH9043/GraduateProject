#include "EngineDefines.h"
#include "UScreenRenderObj.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "UShader.h"
#include "UVIBufferRect.h"
#include "URenderTargetGroup.h"
#include "UTransform.h"
#include "UMethod.h"

_uint			UScreenRenderObj::s_ScreenRenderObjCnt;

UScreenRenderObj::UScreenRenderObj(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::NON), 
	m_eScreenObjGroupID{RTGROUPID::ENDGROUP},
	m_iRegisterCamID{0},
	m_spScreenRenderTargetGroup{nullptr},
	m_spVIBufferRect{nullptr}
{
}

UScreenRenderObj::UScreenRenderObj(const UScreenRenderObj& _rhs) : 
	UPawn(_rhs), 
	m_eScreenObjGroupID{ RTGROUPID::ENDGROUP },
	m_iRegisterCamID{ 0 },
	m_spScreenRenderTargetGroup{nullptr},
	m_spVIBufferRect{ nullptr }
{
}

void UScreenRenderObj::Free()
{
	--s_ScreenRenderObjCnt;
}

HRESULT UScreenRenderObj::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UScreenRenderObj::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	RETURN_CHECK(_vecDatas.size() <= 0, E_FAIL);

	if (_vecDatas.size() > 0)
	{
		m_wpOwnerPawn = UMethod::ConvertTemplate_Index<SHPTR<UPawn>>(_vecDatas, 0);
	}

	AddShader(PROTO_RES_SCREENRENDERONBJSHADER);
	m_spVIBufferRect = AddResource<UVIBufferRect>(PROTO_RES_VIBUFFERRECT, RES_VIBUFFER);

	m_eScreenObjGroupID = static_cast<RTGROUPID>((_uint)(RTGROUPID::SCREENRENDER_DEFFRED_START) +
		++s_ScreenRenderObjCnt);


	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	m_spScreenRenderTargetGroup = spGameInstance->FindRenderTargetGroup(m_eScreenObjGroupID);
	if (nullptr == m_spScreenRenderTargetGroup)
	{
		SHPTR<GRAPHICDESC> GraphicDesc = spGameInstance->GetGraphicDesc();
		// NonAlpha Final 
		{
			std::vector<RTDESC> vecRts{
				RTDESC{ RTOBJID::SCREEN_RENDER_DEFFERED, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,
					GraphicDesc->iWinCX, GraphicDesc->iWinCY, { 0.f, 0.f, 0.f, 0.f } }
			};
			// Add RenderTargetGroup
			spGameInstance->AddRenderTargetGroup(m_eScreenObjGroupID, vecRts);
			m_spScreenRenderTargetGroup = spGameInstance->FindRenderTargetGroup(m_eScreenObjGroupID);
		}
	}
	return S_OK;
}

void UScreenRenderObj::TickActive(const _double& _dTimeDelta)
{
}

void UScreenRenderObj::LateTickActive(const _double& _dTimeDelta)
{
	AddRenderGroup(RENDERID::RI_TARGETS);
}

HRESULT UScreenRenderObj::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->ChangeRenderCamera(m_iRegisterCamID);	

	GetRenderer()->RenderOtherCamera();

	m_spScreenRenderTargetGroup->WaitResourceToTarget(_spCommand);
	m_spScreenRenderTargetGroup->ClearRenderTargetView(_spCommand);
	m_spScreenRenderTargetGroup->OmSetRenderTargets(_spCommand);
	// PipeLineState, TableDescriptor
	GetShader()->SettingPipeLineState(_spCommand);
	__super::RenderActive(_spCommand, _spTableDescriptor);
	// Bind Transform Data 
	GetRenderer()->BindDefferedTransform(GetShader());
	//  Diffuse Texture 가져와서 Bind 
	SHPTR<URenderTargetGroup> spNonAlpha = spGameInstance->FindRenderTargetGroup(RTGROUPID::NONALPHA_DEFFERED);
	GetShader()->BindSRVBuffer(SRV_REGISTER::T0, spNonAlpha->GetRenderTargetTexture(RTOBJID::NONALPHA_DIFFUSE_DEFFERED));
	SHPTR<URenderTargetGroup> spLightGroup = spGameInstance->FindRenderTargetGroup(RTGROUPID::LIGHTSHADE_DEFFERED);
	GetShader()->BindSRVBuffer(SRV_REGISTER::T1, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_AMBIENT_DEFFERED));
	GetShader()->BindSRVBuffer(SRV_REGISTER::T2, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SHADE_DEFFERED));
	GetShader()->BindSRVBuffer(SRV_REGISTER::T3, spLightGroup->GetRenderTargetTexture(RTOBJID::LIGHTSHADE_SPECULAR_DEFFERED));

	//  Diffuse Texture 가져와서 Bind 
	GetShader()->BindSRVBuffer(SRV_REGISTER::T4, spGameInstance->
		FindRenderTargetTexture(RTGROUPID::ALPHA_DEFFERED,
			RTOBJID::ALPHA_DIFFUSE_DEFFERED));

	m_spVIBufferRect->Render(GetShader(), _spCommand);
	m_spScreenRenderTargetGroup->WaitTargetToResource(_spCommand);

//	m_wpOwnerPawn.lock()->AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
	return S_OK;
}

void UScreenRenderObj::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
