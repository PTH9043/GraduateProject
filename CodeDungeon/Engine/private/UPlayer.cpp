#include "EngineDefine.h"
#include "UPlayer.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UCamera.h"
#include "UStageManager.h"
#include "UStage.h"
#include "URegion.h"
#include "UNavigation.h"
#include "UAnimationController.h"

UPlayer::UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCharacter(_spDevice, _wstrLayer, _eCloneType)
{
}

UPlayer::UPlayer(const UPlayer& _rhs) : 
	UCharacter(_rhs)
{
}

void UPlayer::Free()
{
}

HRESULT UPlayer::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UPlayer::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	// VOIDDATAS에 CharacterDesc 0번, PlayerDesc 1번에 값을 채워야한다. 
	assert(_Datas.size() > 1);

	PLAYERDESC PlayerDesc = UMethod::ConvertTemplate_Index<PLAYERDESC>(_Datas, PLAYERDESCORDER);
	assert(nullptr != PlayerDesc.spFollowCamera);

	m_spFollowCamera = PlayerDesc.spFollowCamera;
	assert(nullptr != m_spFollowCamera);

	m_wpCurRegion = PlayerDesc.spStageManager->GetStage()->GetRegion(0);
	assert(nullptr != m_wpCurRegion.lock());

	SHPTR<URegion> spCurRegion = m_wpCurRegion.lock();
	SHPTR<UNavigation> spNavigation = spCurRegion->GetNavigation();

	SHPTR<UCell> spCell = spNavigation->FindCell({_float3{-167.f, -80.54f, 133.f}});
	GetTransform()->SetPos(spCell->GetCenterPos());
	return S_OK;
}

void UPlayer::TickActive(const _double& _dTimeDelta)
{
	GetAnimationController()->Tick(_dTimeDelta);

	__super::TickActive(_dTimeDelta);
}

void UPlayer::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	// Region 
	{
		SHPTR<URegion> spCurRegion = m_wpCurRegion.lock();
		SHPTR<UNavigation> spNavigation = spCurRegion->GetNavigation();
		_float3 vPosition{ GetTransform()->GetPos() };
		SHPTR<UCell> spCell{};

		if (false == spNavigation->IsMove(vPosition, REF_OUT spCell))
		{
			GetTransform()->SetPos(GetPrevPos());
		}
	}
	// Camera 
	{
		_float3 vPosition = DirectX::XMVector3Transform(_float3(0.f, 1000.f, -1000.f), GetTransform()->GetWorldMatrix());
		m_spFollowCamera->GetTransform()->SetPos(vPosition);
		m_spFollowCamera->GetTransform()->LookAt(GetTransform()->GetPos());
	}
}

HRESULT UPlayer::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT UPlayer::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void UPlayer::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

