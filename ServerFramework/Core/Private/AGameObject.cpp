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
		m_spTransform{ Create<ATransform>() }, m_CellIndex{0}, m_fMoveSpeed{0.f}, m_fRunSpeed{0.f}
		, m_isActive{false}, m_isPermanentDisable{false}, m_CopyHead{}
	{
		MemoryInitialization(m_CopyBuffer.data(), MAX_BUFFER_LENGTH);
	}

	_bool AGameObject::Start(const VOIDDATAS& _ReceiveDatas)
	{
		return true;
	}

	void AGameObject::Tick(const _double& _dTimeDelta)
	{
	}

	void AGameObject::RunningPermanentDisableSituation()
	{
	}

	void AGameObject::InsertColliderContainer(_int _ColliderTag, _int _ColliderType, const COLLIDERDESC& _ColliderDesc)
	{
		m_ColliderContainer.insert(MakePair(_ColliderTag, Create<ACollider>((ACollider::TYPE)_ColliderType,
			_ColliderDesc)));
	}

	void AGameObject::BringSpaceIndex(SHPTR<ASpace> _spSpace)
	{
		RETURN_CHECK(nullptr == _spSpace, ;);
		m_SpaceIndex = _spSpace->GetSpaceIndex();
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
		_float fYPos = (vPos.y - _OtherPos.y);
		if (std::abs(fYPos) >= 30.f)
		{
			SetActive(false);
			return false;
		}

		_float fLength = Vector3(vPos - _OtherPos).Length();

		if (fLength <= SEE_RANGE_TICKACTIVE)
		{
			if (fLength <= SEE_RANGE)
			{
				return true;
			}
			SetActive(true);
		}
		else
		{
			SetActive(false);
		}
		return false;
	}

	_bool AGameObject::IsCanSee(SHPTR<ATransform> _spTransform)
	{
		assert(nullptr != _spTransform);
		return IsCanSee(_spTransform->GetPos());
	}

	const _bool AGameObject::IsPermanentDisable() const
	{
		return m_isPermanentDisable;
	}

	void AGameObject::SetActive(const _bool _isActive)
	{
		m_isActive = _isActive;
		if (true == _isActive)
		{
			CallActiveEnable();
		}
		else
		{
			CallActiveDisable();
		}
	}

	void AGameObject::ActivePermanentDisable()
	{
		if (true == m_isPermanentDisable)
			return;

		m_isPermanentDisable = true;
		SetActive(false);
		LastBehavior();
	}

	void AGameObject::LastBehavior()
	{
	}

	void AGameObject::CallActiveEnable()
	{
	}

	void AGameObject::CallActiveDisable()
	{
	}

	void AGameObject::RestrictPositionToNavi()
	{
	}

	void AGameObject::UpdateColliderData()
	{
		SHPTR<ATransform> spTransform = GetTransform();
		for (auto& iter : m_ColliderContainer)
		{
			iter.second->SetTransform(spTransform);
		}
	}

	void AGameObject::Free()
	{
	}
}