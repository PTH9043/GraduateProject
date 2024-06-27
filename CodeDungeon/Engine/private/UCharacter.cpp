#include "EngineDefine.h"
#include "UCharacter.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "UMethod.h"
#include "UGameInstance.h"
#include "UAnimationController.h"
#include "UStageManager.h"
#include "URegion.h"
#include "UNavigation.h"
#include "UStage.h"
#include "UCell.h"
#include "UCollider.h"

UCharacter::UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC, PAWNTYPE::PAWN_CHAR),
	m_spAnimModel{ nullptr }, m_spAnimationController{ nullptr }, m_vPrevPos{}, 	m_spCurNavi{nullptr }, m_spHitCollider{nullptr}, 
	m_fMoveSpeed{0.f}, m_fRunSpeed{0.f}, m_bIsRunning{false}, m_bisHit{false}, m_bisCollision{false},
	m_isNetworkConnected{false}
{
}

UCharacter::UCharacter(const UCharacter& _rhs) : UPawn(_rhs), m_vPrevPos{}, m_bisHit{ false }, m_bisCollision{ false }
{
}

void UCharacter::Free()
{
}

HRESULT UCharacter::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UCharacter::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	assert(_Datas.size() > 0);

	CHARACTERDESC CharacterDesc = UMethod::ConvertTemplate_Index<CHARACTERDESC>(_Datas, CHARACTERDESCORDER);
	assert(false == CharacterDesc.wstrAnimControllerProtoData.empty());
	m_isNetworkConnected = CharacterDesc.isNetworkConnected;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	{
		m_spAnimModel = std::static_pointer_cast<UAnimModel>(spGameInstance->CloneResource(CharacterDesc.wstrAnimModelProtoData));
	}
	{
		UAnimationController::ANIMCONTROLLERDESC ControllerDesc{ ThisShared<UCharacter>() };
		m_spAnimationController = std::static_pointer_cast<UAnimationController>(spGameInstance->CloneComp(CharacterDesc.wstrAnimControllerProtoData, 
			{ &ControllerDesc }));
	}
	{
		UNavigation::NAVDESC navDesc;
		navDesc.iCurIndex = 703;
		m_spCurNavi = std::static_pointer_cast<UNavigation>(spGameInstance->CloneComp(PROTO_NAVI_INTERIOR, {&navDesc }));
		assert(nullptr != m_spCurNavi);
	}
	AddShader(PROTO_RES_ANIMMODELSHADER, RES_SHADER);

	return S_OK;
}

_float UCharacter::OtherCharacterToDistance(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterToDistance(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterDirToLook(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLook(_spOtherActor->GetTransform());
}

_float UCharacter::OhterCharacterDirToRight(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OhterCharacterDirToRight(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterDirToLookConverter(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLookConverter(_spOtherActor->GetTransform());
}

_float3 UCharacter::OtherCharacterDirToLookVectorF3(CSHPTRREF<UActor> _spOtherActor)
{
	assert(nullptr != _spOtherActor);
	return OtherCharacterDirToLookVectorF3(_spOtherActor->GetTransform());
}

_float UCharacter::OtherCharacterToDistance(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != spTransform && nullptr != _spOtherTransform);

	_float fDistance = 0.f;

	fDistance = DirectX::XMVectorGetX(DirectX::XMVector3Length(spTransform->GetPos()
		- _spOtherTransform->GetPos()));

	return fDistance;
}
_float UCharacter::OtherCharacterDirToLook(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != _spOtherTransform && nullptr != spTransform);

	_float3 v3Look;
	v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
	return v3Look.Dot(spTransform->GetLook());
}
_float UCharacter::OhterCharacterDirToRight(CSHPTRREF<UTransform> _spOtherTransform) 
{
	SHPTR<UTransform> spTransform = GetTransform();
	assert(nullptr != _spOtherTransform && nullptr != spTransform);

	_float3 v3Look;
	v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
	return v3Look.Dot(spTransform->GetRight());
}
_float UCharacter::OtherCharacterDirToLookConverter(CSHPTRREF<UTransform> _spOtherTransform) 
{
	_float fDot = DirectX::XMConvertToDegrees(acosf(OtherCharacterDirToLook(_spOtherTransform)));
	_float fValue = OhterCharacterDirToRight(_spOtherTransform);

	if (fValue < 0)
		fDot = -fDot;

	return fDot;
}

_float3 UCharacter::OtherCharacterDirToLookVectorF3(CSHPTRREF<UTransform> _spOtherTransform) 
{
	_float3 vDirection = _float3(0.f, 0.f, 0.f);
	assert(nullptr != _spOtherTransform);
	vDirection = _spOtherTransform->GetPos() - GetTransform()->GetPos();
	vDirection.Normalize();
	return vDirection;
}

void UCharacter::TickActive(const _double& _dTimeDelta)
{
	// ���� ��ġ ����
	m_vPrevPos = GetTransform()->GetPos();

	__super::TickActive(_dTimeDelta);
}

void UCharacter::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
	m_f3MovedDirection = GetTransform()->GetPos() - m_vPrevPos;
	m_f3MovedDirection.Normalize();

	if (!m_f3MovedDirection.Length() == 0.0f)
		m_f3LastMovedDirection = m_f3MovedDirection;
}

HRESULT UCharacter::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (nullptr != m_spAnimModel)
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);

		for (_uint i = 0; i < m_spAnimModel->GetMeshContainerCnt(); ++i)
		{
			// Bind Transform 
			GetTransform()->BindTransformData(GetShader());

			m_spAnimModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
			m_spAnimModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
			// Render
			m_spAnimModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

HRESULT UCharacter::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void UCharacter::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}

HRESULT UCharacter::MakeCollider(const _float3& _vTranslate, const _float3& _vScale, const _int _ColliderType)
{
	return E_NOTIMPL;
}

void UCharacter::ApplySlidingMovement(const _float3& _collidedNormal, _float _speed,  _float _deltaTime)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_float3 currentPosition = GetTransform()->GetPos();
	_float3 movementDirection = currentPosition - GetPrevPos();
	
	float dotProduct = DirectX::XMVector3Dot(XMLoadFloat3(&movementDirection), XMLoadFloat3(&_collidedNormal)).m128_f32[0];

	_float3 slidingVector = movementDirection - _collidedNormal * dotProduct;

	_float3 offset = _collidedNormal * 0.01f;

	// 충돌 보정 및 슬라이딩 벡터 적용
	_float3 newPosition = GetPrevPos() + (slidingVector * _speed * _deltaTime) ;

	// 위치 업데이트
	GetTransform()->SetPos(newPosition);
}

