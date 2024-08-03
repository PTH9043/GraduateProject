#include "CoreDefines.h"
#include "APawn.h"
#include "AAnimController.h"
#include "ACell.h"
#include "ATransform.h"
#include "ANavigation.h"
#include "ACoreInstance.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		AGameObject(OBJCON_CONDATA, _ID, _SessionType), m_spAnimController{nullptr}, m_spNavigation{nullptr}, m_isDamaged{false},
		m_isDead{false}, m_vPrevPosition{}, m_isDeadStateEnable{false}
	{
		m_spNavigation = GetCoreInstance()->CloneNavi();
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
			SHPTR<ACell> spNewCell{};

			if (false == spNaviagation->IsMove(vPosition, spNewCell))
			{
				spTransform->SetPos(m_vPrevPosition);
			}
			else
			{
				spNaviagation->ComputeHeight(vPosition);
				spTransform->SetPos(vPosition);
			}
		}
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

	bool APawn::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void APawn::HealHp(const _float _fHeal)
	{
		m_CharStatus.fHp += _fHeal;
		if (m_CharStatus.fHp >= m_CharStatus.fSaveHp)
			m_CharStatus.fHp = m_CharStatus.fSaveHp;
	}

	void APawn::DamageToEnemy(const _float _fDamge)
	{
		m_CharStatus.fHp -= _fDamge;
		if (m_CharStatus.fHp <= 0)
		{
			m_isDead = true;
		}
	}

	void APawn::Free()
	{
	}

}