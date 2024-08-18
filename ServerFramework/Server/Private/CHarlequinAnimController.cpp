#include "ServerDefines.h"
#include "CHarlequinAnimController.h"
#include "CHarlequin.h"
#include "AAnimation.h"
#include "AAnimator.h"
#include "ATransform.h"
#include "CShurikenThrowing.h"

namespace Server {

	CHarlequinAnimController::CHarlequinAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_wpHarlequin{}, m_isTauntMode{ false }, m_isAttackMode{ false }, m_LastHitTimer{}, m_LastAttackTimer{},
		m_IdleTimer{ 2 }, m_Attack1FirstTime{}, m_IdleRandomValueChoosingTimerForPartrol{ 2 }, m_HitCoolTimer{ 3 }, m_JumpTimer{ 2 },
		m_isStartLastHitTime{ false }, m_isLastAttackWasFirst{ false }, m_vThrowingPos{},
		m_arrThrowindDir{}, m_iRandomValueForPatrol{ 0 }, m_iRandomValueForDoge{ 0 },
		m_isDoge{ false }, m_isWillWalkUntilCloseRange{ false }, m_isWillJumpToCloseRange{ false },
		m_isHitCooldown{ false }, m_iHitCount{ 0 }, m_isJumpEnable{ true }, m_iJumpCounter{ 0 }, m_iMaxJumpCount{ 3 },
		m_RelaxTimer{4}
	{
		m_wpHarlequin = static_shared_cast<CHarlequin>(_spPawn);
		m_RelaxTimer.fTimer = m_RelaxTimer.fStandardTime;
	}

	void CHarlequinAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_patrol(0, 3);
		static std::uniform_int_distribution<> dis_jump(1, 3);
		static std::uniform_int_distribution<> dis_dodge(0, 1);
		static std::uniform_int_distribution<> dis_hit(0, 2);

		static constexpr _float ATTACKRANGE_LONG{ 50 };
		static constexpr _float ATTACKRANGE_JUMP{ 40 };
		static constexpr _float ATTACKRANGE_CLOSE{ 10 };
		static constexpr _float END_CURANIMRATE{ 0.5f };
		static constexpr _float ROTATE_RADIUS{ DirectX::XMConvertToDegrees(120) };

		SHPTR<CHarlequin>	spHarlequin = m_wpHarlequin.lock();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = spHarlequin->GetTransform();

		_string CurAnimName = spAnimation->GetAnimName();
		_bool isHit = spHarlequin->IsDamaged();
		_bool isFoundPlayer = spHarlequin->IsCurrentFindPlayer();
		_bool isDisableFoundPlayer = spHarlequin->IsCurrentNotFound();
		_bool isAtkPlayer = spHarlequin->IsCurrentAtkPlayer();
		_bool isFirstFound = spHarlequin->IsFoundPlayerFirstTime();
		_bool isDeadState = spHarlequin->IsDead();
		_float fTargetToDistance = spHarlequin->GetDistanceToPlayer();
		_float fProgressRate = spAnimation->GetAnimationProgressRate();
		_float fPlayerToDot = spHarlequin->GetPlayerToDot();
		Vector3 vTargetToDir = spHarlequin->GetTargetToDir();
		_float fRot = spHarlequin->GetPlayerToDot();

		DEFINE_STATIC_CONSTCHAR(IDLE_ORDER, "IDLE");
		DEFINE_STATIC_CONSTCHAR(WALK_ORDER, "WALK");
		DEFINE_STATIC_CONSTCHAR(ATTACK1_ORDER, "ATTACK1");
		DEFINE_STATIC_CONSTCHAR(ATTACK2_ORDER, "ATTACK2");
		DEFINE_STATIC_CONSTCHAR(ATTACK3_ORDER, "ATTACK3");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ORDER, "ATTACK4");
		DEFINE_STATIC_CONSTCHAR(DOGE_ORDER, "DODGE");
		DEFINE_STATIC_CONSTCHAR(TAUNT_ORDER, "TAUNT");
		DEFINE_STATIC_CONSTCHAR(JUMP_ORDER, "JUMP");
		DEFINE_STATIC_CONSTCHAR(CROUCH_ORDER, "CROUCH");

		DEFINE_STATIC_CONSTCHAR(IDLE_ANIMNAME, "idle");
		DEFINE_STATIC_CONSTCHAR(IDLEBREAK_ANIMNAME, "Idle break");
		DEFINE_STATIC_CONSTCHAR(CROUCH_ANIMNAME, "Crouch Idle");
		DEFINE_STATIC_CONSTCHAR(GETHIT_ANIMNAME, "Get Hit");
		DEFINE_STATIC_CONSTCHAR(JUMPFORWARD_ANIMNAME, "Jump Forward");
		DEFINE_STATIC_CONSTCHAR(ATTACK1_ANIMNAME, "Attack 1");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ANIMNAME, "Attack 4");
		DEFINE_STATIC_CONSTCHAR(WALK_ANIMNAME, "Walk");
		DEFINE_STATIC_CONSTCHAR(WALKB_ANIMNAME, "Walk B");

		if (false == isDeadState)
		{
			if (true == isFoundPlayer)
			{
				if (false == isFirstFound)
				{
					Vector3 vLookDir = vTargetToDir;
					vLookDir.y = 0.f;
					spTransform->LookAt(vLookDir + spTransform->GetPos());
				}
				spHarlequin->SetFoundPlayerFirstTime(true);
			}

			if (true == isFirstFound)
			{
				if (false == isFoundPlayer)
				{
					m_isAttackMode = false;
					m_isTauntMode = false;

					if (CurAnimName == CROUCH_ANIMNAME)
					{
						spAnimator->SetAnimation(IDLE_ANIMNAME);
						SetAnimState(MOB_IDLE_STATE);
					}
					else
					{
						UpdateState(IDLE_ORDER, MOB_IDLE_STATE);
					}
				}
				else
				{
					if (false == m_isAttackMode)
					{
						m_isTauntMode = true;
						m_IdleTimer.ResetTimer();
					}
				}
			}

			if (true == m_isTauntMode)
			{
				UpdateState(TAUNT_ORDER, MOB_TAUNT_STATE);
				if (IDLEBREAK_ANIMNAME == CurAnimName)
				{
					m_isAttackMode = true;
					m_isTauntMode = false;
				}
			}

			if (true == isHit && false == m_isHitCooldown)
			{
				if (m_iHitCount < 3)
				{
					spAnimator->SetAnimation(GETHIT_ANIMNAME);
					SetAnimState(MOB_HIT_STATE);
					++m_iHitCount;
				}
				else
				{
					m_isHitCooldown = true;
				}
			}

			if (true == m_isHitCooldown)
			{
				if (true == m_HitCoolTimer.IsOver(_dTimeDelta))
				{
					m_iHitCount = 0;
					m_isHitCooldown = false;
					m_HitCoolTimer.ResetTimer();
				}
			}

			if (true == m_isAttackMode && false == isHit)
			{
				if (fTargetToDistance > ATTACKRANGE_LONG)
				{
					if (CurAnimName != CROUCH_ANIMNAME)
						UpdateState(ATTACK1_ORDER, MOB_ATTACK_STATE);
					else
						UpdateState(ATTACK4_ORDER, MOB_ATTACK_STATE);
				}
				else if (fTargetToDistance > ATTACKRANGE_JUMP && fTargetToDistance <= ATTACKRANGE_LONG)
				{
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						UpdateState(JUMP_ORDER, MOB_MOVE_STATE);
					}
					else
					{
						UpdateState(WALK_ORDER, MOB_MOVE_STATE);
					}
				}
				else if (fTargetToDistance > ATTACKRANGE_CLOSE && fTargetToDistance <= ATTACKRANGE_JUMP)
				{
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						UpdateState(ATTACK4_ORDER, MOB_ATTACK_STATE);
					}
					else
					{
						UpdateState(JUMP_ORDER, MOB_MOVE_STATE);
					}
				}
				else
				{
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						UpdateState(ATTACK3_ORDER, MOB_ATTACK_STATE);
					}
					else
					{
						if (fProgressRate >= END_CURANIMRATE)
							m_iRandomValueForDoge = dis_dodge(gen);

						if (0 == m_iRandomValueForDoge)
						{
							UpdateState(ATTACK2_ORDER, MOB_ATTACK_STATE);
						}
						else
						{
							UpdateState(DOGE_ORDER, MOB_ATTACK_STATE);
						}
					}
				}
			}

			if (JUMPFORWARD_ANIMNAME == CurAnimName)
			{
				if (fProgressRate >= 0.0f && fProgressRate <= 0.2f)
				{
					spHarlequin->SetDirectionFixedUp(_dTimeDelta, fRot, 10, spHarlequin->GetTargetPos());
				}

				if (fProgressRate >= 0.6f)
				{
					UpdateState(CROUCH_ORDER, MOB_IDLE_STATE);
				}
			}
			else if (ATTACK1_ANIMNAME == CurAnimName)
			{
				spHarlequin->SetDirectionFixedUp(_dTimeDelta, fRot, 10, spHarlequin->GetTargetPos());
			}
			else if (ATTACK4_ANIMNAME == CurAnimName)
			{
				if (fProgressRate >= 0.75f)
				{
					UpdateState(WALK_ORDER, MOB_MOVE_STATE);
				}
			}
		}
		else
		{
			UpdateState("DEATH", MOB_DEATH_STATE);
		}

		// Tick Event
		spAnimator->TickEvent(spHarlequin.get(), GetInputTrigger(), _dTimeDelta);
	}

	void CHarlequinAnimController::Free()
	{
	}


}