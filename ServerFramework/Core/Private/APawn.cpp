#include "CoreDefines.h"
#include "APawn.h"
#include "AAnimController.h"
#include "ACell.h"
#include "ATransform.h"
#include "ANavigation.h"
#include "ACoreInstance.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) :
		AGameObject(OBJCON_CONDATA, _ID, _SessionType), m_spAnimController{ nullptr }, m_spNavigation{ nullptr }, m_isDamaged{ false },
		m_isDead{ false }, m_vPrevPosition{}, m_isDeadStateEnable{ false }, m_isEnemyHitState{false},
		m_isHitAlreadyState{false}, m_isCollisionState{false}, m_DeadTimer{10}, m_DamageToEnemyTimer{0.2f, std::memory_order_seq_cst}
	{
		m_spNavigation = GetCoreInstance()->CloneNavi();
		m_DamageToEnemyTimer.fTimer = m_DamageToEnemyTimer.fStandardTime;
	}

	SHPTR<ACell> APawn::GetCurCell()
	{
		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ACell> spCell = spNavigation->GetCurCell();
		if (nullptr == spCell)
		{
			SHPTR<ATransform> spTransform = GetTransform();
			spNavigation->IsMove(spTransform->GetPos(), spCell);
		}
		return spCell;
	}

	void APawn::SetActive(const _bool _isActive)
	{
		__super::SetActive(_isActive);
	}

	void APawn::RestrictPositionToNavi()
	{
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<ANavigation> spNaviagation = GetNavigation();
		{
			Vector3 vPosition = spTransform->GetPos();
			spNaviagation->ComputeHeight(REF_OUT vPosition);
	//		spTransform->SetPos(vPosition);

			SHPTR<ACell> spNewCell{};
			if (false == spNaviagation->IsMove(vPosition, REF_OUT spNewCell))
			{
				Vector3 closestPoint = spNaviagation->ClampPositionToCell(vPosition);
				closestPoint.y = vPosition.y;
				vPosition = closestPoint;
			}

			spTransform->SetPos(vPosition);
		}
	}

	void APawn::RunningDamagedToEnemyTimer(const _double& _dTimeDelta)
	{
		m_DamageToEnemyTimer.PlusTime(_dTimeDelta);
	}

	void APawn::Placement(_int _CellIndex)
	{
		SHPTR<ANavigation> spNavigation = m_spNavigation;
		SHPTR<ACell> spCell = spNavigation->FindCell(_CellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
	}

	Vector3 APawn::BringCellIndextoPosition()
	{
		SHPTR<ANavigation> spNavigation = m_spNavigation;
		SHPTR<ACell> spCell = spNavigation->FindCell(GetTransform()->GetPos());
		if (nullptr == spCell)
		{
			return GetTransform()->GetPos();
		}
		SetCellIndex(spCell->GetIndex());
		return spCell->GetCenterPos();
	}

	_float APawn::HealHp(const _float _fHeal)
	{
		m_CharStatus.fHp += _fHeal;
		if (m_CharStatus.fHp >= m_CharStatus.fSaveHp)
			m_CharStatus.fHp = m_CharStatus.fSaveHp;

		return m_CharStatus.fHp;
	}

	_float APawn::DamageToEnemy(SHPTR<APawn> _spPawn)
	{
		assert(nullptr != _spPawn);
		RETURN_CHECK(true == m_isDamaged, 0.f);
		m_CharStatus.fHp -= _spPawn->GetCharStatus().fPower;
		if (m_CharStatus.fHp <= 0)
		{
			m_isDead = true;
		}
		m_isDamaged = true;
		return m_CharStatus.fHp;
	}

	_float APawn::Damaged(float _fDamaged)
	{
		m_CharStatus.fHp -= _fDamaged;
		if (m_CharStatus.fHp <= 0)
		{
			m_isDead = true;
		}
		m_isDamaged = true;
		return m_CharStatus.fHp;
	}

	_float APawn::OtherCharacterToDistance(SHPTR<ATransform> _spOtherTransform)
	{
		assert(nullptr != _spOtherTransform);
		return OtherCharacterToDistance(_spOtherTransform->GetPos());
	}
	_float APawn::OtherCharacterDirToLook(SHPTR<ATransform> _spOtherTransform)
	{
		assert(nullptr != _spOtherTransform);
		return OtherCharacterDirToLook(_spOtherTransform->GetPos());
	}
	_float APawn::OhterCharacterDirToRight(SHPTR<ATransform> _spOtherTransform)
	{
		assert(nullptr != _spOtherTransform);
		return OhterCharacterDirToRight(_spOtherTransform->GetPos());
	}
	_float APawn::OtherCharacterDirToLookConverter(SHPTR<ATransform> _spOtherTransform)
	{
		assert(nullptr != _spOtherTransform);
		return OtherCharacterDirToLookConverter(_spOtherTransform->GetPos());
	}
	Vector3 APawn::OtherCharacterDirToLookVectorF3(SHPTR<ATransform> _spOtherTransform)
	{
		assert(nullptr != _spOtherTransform);
		return OtherCharacterDirToLookVectorF3(_spOtherTransform->GetPos());
	}

	_float APawn::OtherCharacterToDistance(Vector3 _targetPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		_float fDistance = 0.f;

		fDistance = DirectX::XMVectorGetX(DirectX::XMVector3Length(spTransform->GetPos()
			- _targetPos));

		return fDistance;
	}

	_float APawn::OtherCharacterDirToLook(Vector3 _targetPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		assert(nullptr != spTransform);

		Vector3 v3Look;
		v3Look = (_targetPos - spTransform->GetPos());
		return v3Look.Dot(spTransform->GetLook());
	}

	_float APawn::OhterCharacterDirToRight(Vector3 _targetPos)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		assert(nullptr != spTransform);

		Vector3 v3Look;
		v3Look = (_targetPos - spTransform->GetPos());
		return v3Look.Dot(spTransform->GetRight());
	}

	_float APawn::OtherCharacterDirToLookConverter(Vector3 _targetPos)
	{
		_float fDot = DirectX::XMConvertToDegrees(acosf(OtherCharacterDirToLook(_targetPos)));
		_float fValue = OhterCharacterDirToRight(_targetPos);

		if (fValue < 0)
			fDot = -fDot;

		return fDot;
	}

	Vector3 APawn::OtherCharacterDirToLookVectorF3(Vector3 _targetPos)
	{
		Vector3 vDirection = Vector3(0.f, 0.f, 0.f);
		vDirection = _targetPos - GetTransform()->GetPos();
		vDirection.Normalize();
		return vDirection;
	}

	void APawn::DeadStateEnable(const _double& _dTimeDelta)
	{
		if (true == IsDead())
		{
			if (true == m_DeadTimer.IsOver(_dTimeDelta))
			{
				m_isDeadStateEnable = true;
				ActivePermanentDisable();
			}
		}
	}

	void APawn::Free()
	{
	}

}