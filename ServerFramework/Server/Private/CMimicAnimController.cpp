#include "ServerDefines.h"
#include "CMimicAnimController.h"
#include "ATransform.h"
#include "CMimic.h"
#include "AAnimation.h"
#include "AAnimator.h"

namespace Server {

	CMimicAnimController::CMimicAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_isAttackMode{ false }, m_IdleTimer{ 2 }, m_IdleRandomValueChooseTimer{ 2 }
	{
		m_wpMimic = static_shared_cast<CMimic>(_spPawn);
	}

	void CMimicAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_patrol(0, 3);

		SHPTR<CMimic>	spMimic = m_wpMimic.lock();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		_string CurAnimName = spAnimation->GetAnimName();
		_bool isHit = spMimic->IsDamaged();
		_bool isFoundPlayer = spMimic->IsCurrentFindPlayer();
		_bool isDisableFoundPlayer = spMimic->IsCurrentNotFound();
		_bool isAtkPlayer = spMimic->IsCurrentAtkPlayer();
		_bool isFirstFound = spMimic->IsFoundPlayerFirstTime();
		_bool isDeadState = spMimic->IsDead();
		_bool isDeadStateEnable = spMimic->IsDeadStateEnable();
		_bool isPressKeyEnable = spMimic->IsPressKeyEnable();
		_bool isOpeningState = spMimic->IsOpeningState();
		_float fTargetToDistance = spMimic->GetDistanceToPlayer();

		DEFINE_STATIC_CONSTCHAR(IDLE_ANIMNAME, "Idle");
		DEFINE_STATIC_CONSTCHAR(HIT_ANIMNAME, "Hit");
		DEFINE_STATIC_CONSTCHAR(IDLE_ORDER, "IDLE");
		DEFINE_STATIC_CONSTCHAR(WALK_ORDER, "WALK");
		DEFINE_STATIC_CONSTCHAR(ATTACK_ORDER, "ATTACK");

		if (false == isDeadState)
		{
			if (true == isFoundPlayer && false == isOpeningState)
			{
				if (true == isPressKeyEnable)
				{
					spMimic->EnableOpen();
					spAnimator->SetAnimation("Enter");
				}
			}
			else if (false == isFoundPlayer && true == isOpeningState)
			{
				m_isAttackMode = false;
				if (IDLE_ANIMNAME == CurAnimName)
				{
					if (true == m_IdleTimer.IsOver(_dTimeDelta))
					{
						m_IdleTimer.ResetTimer();
					}
				}
				if (0 == m_IdleTimer.fTimer)
				{
					if (true == m_IdleRandomValueChooseTimer.IsOver(_dTimeDelta))
					{
						_int iRandomValue = dis_patrol(gen);
						if (0 == iRandomValue)
							UpdateState(IDLE_ORDER, MOB_IDLE_STATE);
						else
							UpdateState(WALK_ORDER, MOB_MOVE_STATE);
					}
				}
			}
			else if (true == isFoundPlayer && true == isOpeningState)
			{
				m_isAttackMode = true;
			}

			if (true == isHit)
			{
				spAnimator->SetAnimation(HIT_ANIMNAME);
				SetAnimState(MOB_HIT_STATE);
			}
			else if (true == m_isAttackMode)
			{
				if (false == isAtkPlayer)
				{
					UpdateState(WALK_ORDER, MOB_MOVE_STATE);
				}
				else
				{
					UpdateState(ATTACK_ORDER, MOB_ATTACK_STATE);
				}
			}
		}
		else
		{
			UpdateState("DEATH", MOB_DEATH_STATE);
		}
		// Tick Event
		spAnimator->TickEvent(spMimic.get(), GetInputTrigger(), _dTimeDelta);
	}

	void CMimicAnimController::Free()
	{
	}


}