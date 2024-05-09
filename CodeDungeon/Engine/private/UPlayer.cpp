#include "EngineDefine.h"
#include "UPlayer.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UCamera.h"
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
	return S_OK;
}

void UPlayer::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void UPlayer::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
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

void UPlayer::FollowCameraMove(const _float3& _vPlayerToDistancePosition)
{
	_float3 vPosition = DirectX::XMVector3Transform(_vPlayerToDistancePosition, GetTransform()->GetWorldMatrix());
	m_spFollowCamera->GetTransform()->SetPos(vPosition);
	m_spFollowCamera->GetTransform()->LookAt(GetTransform()->GetPos());
}

