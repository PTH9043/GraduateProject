#include "ServerDefines.h"
#include "CMinotaurAnimController.h"
#include "ATransform.h"
#include "CMinotaur.h"
#include "AAnimation.h"
#include "AAnimator.h"
#include "ASession.h"

namespace Server {

	CMinotaurAnimController::CMinotaurAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_wpMinotaur{}, m_isAttackMode{false}, m_isRushMode{false},
		m_LastHitTimer{5}, m_LastAttackTimer{3}, m_IdleRandomValueChooseTimer{},
		m_RushModeTimer{2.5}, m_RushAttackTimer{1.5}, m_HitCooldownTimer{5},
		m_isStartlastHitTime{false}, m_isLastAttackWasFirst{false}, m_iRandomValue{0},
		m_vRushTargetPos{}, m_iRandomNumForHit{0},	m_iHitCount{0}, m_ChangeStateTimer{3}
	{
		m_wpMinotaur = static_shared_cast<CMinotaur>(_spPawn);
		m_ChangeStateTimer.fTimer = 3.f;
	}

	void CMinotaurAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_rush(0, 10);
		static std::uniform_int_distribution<> dis_attack(0, 10);

		SHPTR<CMinotaur> spMinotaur = m_wpMinotaur.lock();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		SHPTR<ASession> spSession = spMinotaur->GetTargetSession();
		_int iPlayerState = 0;
		_bool isPlayerFallDown = false;
		if (nullptr != spSession)
		{
			iPlayerState = spSession->GetCurrentAnimState();
			isPlayerFallDown = spSession->IsFallDownState();
		}
		_string CurAnimName = spAnimation->GetAnimName();
		_bool isHit = spMinotaur->IsDamaged();
		_bool isFoundPlayer = spMinotaur->IsCurrentFindPlayer();
		_bool isDisableFoundPlayer = spMinotaur->IsCurrentNotFound();
		_bool isAtkPlayer = spMinotaur->IsCurrentAtkPlayer();
		_bool isFirstFound = spMinotaur->IsFoundPlayerFirstTime();
		_bool isDeadState = spMinotaur->IsDead();
		_bool isDeadStateEnable = spMinotaur->IsDeadStateEnable();
		_float fTargetToDistance = spMinotaur->GetDistanceToPlayer();


		static const char* IDLEORDER = "IDLE";
		static const char* WALKORDER_1 = "WALK";
		static const char* WALKORDER_2 = "WALK2";
		static const char* ATTACKORDER = "ATTACK";
		static const char* KICKORDER_1 = "KICK_1";
		static const char* KICKORDER_2 = "KICK_2";
		static const char* WALKBACK_ORDER = "WALKBACK";
		static const char* RUNORDER = "RUN";

		static const char* HITANIMNAME = "hit_1";

		if (false == isDeadState)
		{
			if (true == isFirstFound)
			{
				if (false == isFoundPlayer)
				{
					m_isAttackMode = false;
					UpdateState(WALKORDER_1, MOB_MOVE_STATE);
				}
				else
				{
					if (false == m_isRushMode)
						m_isAttackMode = true;
				}

				if (true == isHit)
				{
					if (m_iHitCount < 3)
					{
						spAnimator->SetAnimation(HITANIMNAME);
						SetAnimState(MOB_HIT_STATE);
						++m_iHitCount;
					}
					else
					{
						if (true == m_HitCooldownTimer.IsOver(_dTimeDelta))
						{
							m_iHitCount = 0;
							m_HitCooldownTimer.ResetTimer();
						}
					}
				}
				else if (true == m_isAttackMode && false == m_isRushMode)
				{
					if (true == m_LastAttackTimer.IsOver(_dTimeDelta))
					{
						m_isLastAttackWasFirst = !m_isLastAttackWasFirst;
						m_LastAttackTimer.ResetTimer();
					}

					if (false == isAtkPlayer)
					{
						if (m_ChangeStateTimer.IsOver(_dTimeDelta))
						{
							_bool isTrue = dis_rush(gen) >= 5 ? true : false;
							if (false == isTrue)
								m_isRushMode = true;
							m_ChangeStateTimer.ResetTimer();
						}

						if (false == m_isRushMode)
						{
							UpdateState(WALKORDER_2, MOB_MOVE_STATE);
						}
					}
					else
					{
						UpdateState(m_isLastAttackWasFirst ? ATTACKORDER : KICKORDER_1, MOB_ATTACK_STATE);
						m_ChangeStateTimer.ResetTimer();
					}
				}

				if (true == m_isRushMode)
				{
					if (false == m_RushModeTimer.IsOver(_dTimeDelta))
					{
						UpdateState(WALKBACK_ORDER, MOB_BACK_STATE);
					}
					else
					{
						if (fTargetToDistance >= RUNNING_DISTANCE)
						{
							UpdateState(RUNORDER, MOB_RUN_STATE);
						}
					}

					if (fTargetToDistance >= RUNNING_DISTANCE)
					{
						UpdateState(RUNORDER, MOB_RUN_STATE);
					}
					else
					{
						if (true == m_RushAttackTimer.IsOver(_dTimeDelta))
						{
							m_isRushMode = false;
							m_RushAttackTimer.ResetTimer();
							m_RushModeTimer.ResetTimer();
						}
						else
						{
							UpdateState(KICKORDER_2, MOB_ATTACK_STATE);
						}
					}
				}


			}
			else
			{
				if (true == isFoundPlayer)
				{
					spMinotaur->SetFoundPlayerFirstTime(true);
				}
			}
		}
		else
		{
			SetAnimation("death");
			SetAnimState(MOB_DEATH_STATE);
		}

		spAnimator->TickEvent(spMinotaur.get(), GetInputTrigger(), _dTimeDelta);
	}

	void CMinotaurAnimController::Free()
	{
	}


}