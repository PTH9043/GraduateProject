#include "CoreDefines.h"
#include "AGameObject.h"
#include "ACollider.h"
#include "ATransform.h"
#include "ASpace.h"
#include "ACoreInstance.h"
#include "ANavigation.h"
#include "ACell.h"

namespace Core
{
	AGameObject::AGameObject(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		ACoreObject(OBJCON_CONDATA), m_SessionID{_ID}, m_SessionType{_SessionType}, m_SpaceIndex{0},
		m_spTransform{ nullptr }, m_spCollider{ nullptr }, m_CellIndex{0}, m_fMoveSpeed{0.f}, m_fRunSpeed{0.f}
		, m_isActive{false}, m_isPermanentDisable{false}, m_CopyHead{}
	{
		MemoryInitialization(m_CopyBuffer.data(), MAX_BUFFER_LENGTH);
	}

	_bool AGameObject::Start(const VOIDDATAS& _ReceiveDatas)
	{
		m_spTransform = Create<ATransform>();
		return true;
	}

	void AGameObject::CreateColliderAndTransform(const COLLIDERINFO& _ColliderInfo, const Vector3& _vPos)
	{
		m_spCollider = Create<ACollider>((ACollider::TYPE)_ColliderInfo.iColliderType,
			ACollider::COLLIDERDESC{ _ColliderInfo.vPos, _ColliderInfo.vScale });
	}

	void AGameObject::BringSpaceIndex(SHPTR<ASpace> _spSpace)
	{
		RETURN_CHECK(nullptr == _spSpace, ;);
		m_SpaceIndex = _spSpace->GetSpaceIndex();
	}

	void AGameObject::Placement(_int _CellIndex)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<ANavigation> spNavigation = spCoreInstance->GetNavigation();
		SHPTR<ACell> spCell = spNavigation->FindCell(_CellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
	}

	void AGameObject::BringCellIndextoPosition()
	{
		ASSERT_CRASH(nullptr != m_spTransform);
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<ANavigation> spNavigation = spCoreInstance->GetNavigation();
		SHPTR<ACell> spCell = spNavigation->FindCell(GetTransform()->GetPos());
		SetCellIndex(spCell->GetIndex());
	}

	_float AGameObject::OtherCharacterToDistance(SHPTR<ATransform> _spOtherTransform)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		assert(nullptr != spTransform && nullptr != _spOtherTransform);

		_float fDistance = 0.f;

		fDistance = DirectX::XMVectorGetX(DirectX::XMVector3Length(spTransform->GetPos()
			- _spOtherTransform->GetPos()));

		return fDistance;
	}

	_float AGameObject::OtherCharacterDirToLook(SHPTR<ATransform> _spOtherTransform)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		assert(nullptr != _spOtherTransform && nullptr != spTransform);

		Vector3 v3Look;
		v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
		return v3Look.Dot(spTransform->GetLook());
	}

	_float AGameObject::OhterCharacterDirToRight(SHPTR<ATransform> _spOtherTransform)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		assert(nullptr != _spOtherTransform && nullptr != spTransform);

		Vector3 v3Look;
		v3Look = (_spOtherTransform->GetPos() - spTransform->GetPos());
		return v3Look.Dot(spTransform->GetRight());
	}

	_float AGameObject::OtherCharacterDirToLookConverter(SHPTR<ATransform> _spOtherTransform)
	{
		_float fDot = DirectX::XMConvertToDegrees(acosf(OtherCharacterDirToLook(_spOtherTransform)));
		_float fValue = OhterCharacterDirToRight(_spOtherTransform);

		if (fValue < 0)
			fDot = -fDot;

		return fDot;
	}

	Vector3 AGameObject::OtherCharacterDirToLookVectorF3(SHPTR<ATransform>_spOtherTransform)
	{
		Vector3 vDirection = Vector3(0.f, 0.f, 0.f);
		assert(nullptr != _spOtherTransform);
		vDirection = _spOtherTransform->GetPos() - GetTransform()->GetPos();
		vDirection.Normalize();
		return vDirection;
	}

	_bool AGameObject::IsCanSee(Vector3 _OtherPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		Vector3 vPos = spTransform->GetPos();
		vPos.y = 0.f;
		_OtherPos.y = 0.f;
		return Vector3::DistanceSquared(vPos, _OtherPos) < SEE_RANGE;
	}

	_bool AGameObject::IsCanSee(SHPTR<ATransform> _spTransform)
	{
		assert(nullptr != _spTransform);
		return IsCanSee(_spTransform->GetPos());
	}


	void AGameObject::SetActiveWeak(const _bool _isActive)
	{
		bool isActive = m_isActive.load();
		if (m_isActive.compare_exchange_strong(isActive, _isActive))
			return;
	}

	void AGameObject::SetActiveStrong(const _bool _isActive)
	{
		if (IsActive() == _isActive)
			return;

		while (true)
		{
			bool isActive = m_isActive.load();
			if (m_isActive.compare_exchange_strong(isActive, _isActive))
				break;
		}
	}

	void AGameObject::ActivePermanentDisable()
	{
		while (true)
		{
			bool isActive = m_isPermanentDisable.load();
			if (m_isPermanentDisable.compare_exchange_strong(isActive, true))
				break;
		}

		SendLastMessage();
	}

	void AGameObject::SendLastMessage()
	{
	}

	void AGameObject::Free()
	{
	}
}