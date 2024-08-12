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
		m_wpHarlequin{}, m_isTauntMode{ false }, m_isAttackMode { false}, m_LastHitTimer{}, m_LastAttackTimer{},
		m_IdleTimer{2}, m_Attack1FirstTime{}, m_IdleRandomValueChoosingTimerForPartrol{2}, m_HitCoolTimer{3},
		m_isStartLastHitTime{false}, m_isLastAttackWasFirst{false}, m_vThrowingPos{},
		m_arrThrowindDir{},m_iRandomValueForPatrol{0}, m_iRandomValueForDoge{0},
		m_isDoge{ false }, m_isWillWalkUntilCloseRange{ false }, m_isWillJumpToCloseRange{ false },
		m_isHitCooldown{false}, m_iHitCount{0}, m_fRotValue{ DirectX::XMConvertToDegrees(60) }
	{
		m_wpHarlequin = static_shared_cast<CHarlequin>(_spPawn);
	}

	void CHarlequinAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_patrol(0, 3);
		static std::uniform_int_distribution<> dis_attack(0, 1);
		static std::uniform_int_distribution<> dis_dodge(0, 1);
		static std::uniform_int_distribution<> dis_hit(0, 2);

		static constexpr _float ATTACKRANGE_LONG{ 50 };
		static constexpr _float ATTACKRANGE_JUMP{ 40 };
		static constexpr _float ATTACKRANGE_CLOSE{ 10 };
		static constexpr _float END_CURANIMRATE{ 0.9f };

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
		Vector3 vTargetToDir = spHarlequin->GetTargetToDir();

		DEFINE_STATIC_CONSTCHAR(IDLE_ORDER, "IDLE");
		DEFINE_STATIC_CONSTCHAR(WALK_ORDER, "WALK");
		DEFINE_STATIC_CONSTCHAR(ATTACK1_ORDER, "ATTACK1");
		DEFINE_STATIC_CONSTCHAR(ATTACK2_ORDER, "ATTACK2");
		DEFINE_STATIC_CONSTCHAR(ATTACK3_ORDER, "ATTACK3");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ORDER, "ATTACK4");
		DEFINE_STATIC_CONSTCHAR(DOGE_ORDER, "DODGE");
		DEFINE_STATIC_CONSTCHAR(TAUNT_ORDER, "TAUNT");
		DEFINE_STATIC_CONSTCHAR(JUMP_ORDER, "JUMP");
		DEFINE_STATIC_CONSTCHAR(CROUCH_ORDER, "JUMP");

		DEFINE_STATIC_CONSTCHAR(IDLE_ANIMNAME, "Idle");
		DEFINE_STATIC_CONSTCHAR(IDLEBREAK_ANIMNAME, "Idle break");
		DEFINE_STATIC_CONSTCHAR(CROUCH_ANIMNAME, "Crouch Idle");
		DEFINE_STATIC_CONSTCHAR(GETHIT_ANIMNAME, "Get Hit");
		DEFINE_STATIC_CONSTCHAR(JUMPFORWARD_ANIMNAME, "Jump Forward");
		DEFINE_STATIC_CONSTCHAR(ATTACK1_ANIMNAME, "Attack 1");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ANIMNAME, "Attack 4");

		if (false == isDeadState)
		{
			if (true == isFoundPlayer)
			{
				m_isAttackMode = false;
				m_isTauntMode = false;
			}

			if (IDLE_ANIMNAME == CurAnimName)
			{
				if (true == m_IdleTimer.IsOver(_dTimeDelta))
				{
					m_IdleTimer.ResetTimer();
				}
			}

			if (0 == m_IdleTimer.fTimer)
			{
				if (true == m_IdleRandomValueChoosingTimerForPartrol.IsOver(_dTimeDelta))
				{
					m_iRandomValueForPatrol = dis_patrol(gen);
					m_IdleRandomValueChoosingTimerForPartrol.ResetTimer();
				}

				if (0 == m_iRandomValueForPatrol)
				{
					UpdateState(IDLE_ORDER, MOB_IDLE_STATE);
				}
				else
				{
					UpdateState(WALK_ORDER, MOB_MOVE_STATE);
				}
			}

			if (CurAnimName == CROUCH_ANIMNAME)
			{
				spAnimator->SetAnimation(IDLE_ANIMNAME);
				SetAnimState(MOB_IDLE_STATE);
			}

			if (false == m_isAttackMode && true == isFoundPlayer)
			{
				m_isTauntMode = true;
				m_IdleTimer.ResetTimer();
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

			if (true == isHit)
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
				else if (fTargetToDistance >= ATTACKRANGE_JUMP && true == isAtkPlayer)
				{
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						m_isWillJumpToCloseRange = true;
					}
					else
					{
						m_isWillJumpToCloseRange = false;
						UpdateState(WALK_ORDER, MOB_MOVE_STATE);
					}
				}
				else if (fTargetToDistance < ATTACKRANGE_JUMP && fTargetToDistance >= ATTACKRANGE_CLOSE)
				{
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						UpdateState(ATTACK4_ORDER, MOB_ATTACK_STATE);
						m_isWillJumpToCloseRange = false;
					}
					else
					{
						m_isWillJumpToCloseRange = true;
					}
				}
				else if (fTargetToDistance < ATTACKRANGE_CLOSE)
				{
					m_isWillJumpToCloseRange = false;
					if (CurAnimName == CROUCH_ANIMNAME)
					{
						UpdateState(ATTACK3_ORDER, MOB_ATTACK_STATE);
					}
					else
					{
						if (fProgressRate >= END_CURANIMRATE)
							m_iRandomValueForDoge = dis_dodge(gen);

						if(0 == m_iRandomValueForDoge)
							UpdateState(ATTACK2_ORDER, MOB_ATTACK_STATE);
						else 
							UpdateState(DOGE_ORDER, MOB_ATTACK_STATE);
					}
				}
			}

			if (true == m_isWillJumpToCloseRange)
			{
				UpdateState(JUMP_ORDER, MOB_MOVE_STATE);
				m_isWillJumpToCloseRange = false;
			}

			if (JUMPFORWARD_ANIMNAME == CurAnimName)
			{
				if (0.0f <= fProgressRate && 0.1f >= fProgressRate)
				{
					spTransform->SetDirectionFixedUp(vTargetToDir, _dTimeDelta, m_fRotValue);
				}
				else if (fProgressRate >= 0.8f)
				{
					spTransform->SetDirectionFixedUp(vTargetToDir, _dTimeDelta, m_fRotValue);
					UpdateState(CROUCH_ORDER, MOB_IDLE_STATE);
				}
			}
			else if (ATTACK4_ANIMNAME == CurAnimName)
			{
				if (fProgressRate >= 0.8f)
				{
					UpdateState(WALK_ORDER, MOB_MOVE_STATE);
				}
				m_vThrowingPos = spTransform->GetPos();
				m_vThrowingPos.y += 7.f;

				if (fProgressRate >= 0.2f && 0.22f <= fProgressRate)
				{
					for (_int i = 0; i < 6; i++)
					{
						SHPTR<CShurikenThrowing> spShuriken = spHarlequin->GetShurikenThrowing(i);
						if (spShuriken->GetTraveledDistance() == 0)
						{
							m_arrThrowindDir[i] = spTransform->GetLook();
						}
						else
						{
							_float angle = 60.f * (i + 1);
							Vector3 vCurrentLookDir = spTransform->GetLook();
							_float fAngleRadias = angle * angle * (DirectX::XM_PI / 180.0f);
							_float cosAngle = std::cos(fAngleRadias);
							_float sinAngle = std::sin(fAngleRadias);

							_float newX = vCurrentLookDir.x * cosAngle + vCurrentLookDir.z * sinAngle;
							_float newZ = -vCurrentLookDir.x * sinAngle + vCurrentLookDir.z * cosAngle;

							m_arrThrowindDir[i] = Vector3(newX, vCurrentLookDir.y, newZ);
						}

						SHPTR<ATransform> spShurikenTr = spShuriken->GetTransform();
						spShurikenTr->SetPos(m_vThrowingPos);
						spShurikenTr->SetDirection(spTransform->GetRight());
						spShuriken->SetThrown(true);
					}
				}
			}
			else if (ATTACK1_ANIMNAME == CurAnimName)
			{
				m_vThrowingPos = spTransform->GetPos();
				m_vThrowingPos.y += 7.f;

				spTransform->SetDirectionFixedUp(vTargetToDir, _dTimeDelta, m_fRotValue);
				if (fProgressRate >= 0.2f && 0.22f <= fProgressRate 
					|| fProgressRate >= 0.6f && 0.62f <= fProgressRate)
				{
					_int iStart = 0;
					_int iEnd = 3;
					if (fProgressRate >= 0.6f && 0.62f <= fProgressRate)
					{
						iStart = 3;
						iEnd = 6;
					}
					for (_int i = iStart; i < iEnd; ++i)
					{
						SHPTR<CShurikenThrowing> spShuriken = spHarlequin->GetShurikenThrowing(i);
						if (spShuriken->GetTraveledDistance() == 0)
						{
							if (0 == i || 2 == i)
							{
								Vector3 currentLookDir = spTransform->GetLook() * -1.f;
								_float angleRadians = 10 * (DirectX::XM_PI / 180.0f) * (i == 0 ? 1 : -1);
								_float cosAngle = std::cos(angleRadians);
								_float sinAngle = std::sin(angleRadians);

								_float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
								_float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;

								m_arrThrowindDir[i] = Vector3(newX, currentLookDir.y, newZ);
							}
							else if (i == 1)
								m_arrThrowindDir[i] = spTransform->GetLook();

							SHPTR<ATransform> spShurikenTr = spShuriken->GetTransform();
							spShurikenTr->SetPos(m_vThrowingPos);
							spShurikenTr->SetDirection(spTransform->GetRight());
							spShuriken->SetThrown(true);
						}
					}
				}
			}

			for (_int i = 0; i < 6; ++i)
			{
				SHPTR<CShurikenThrowing> spShuriken = spHarlequin->GetShurikenThrowing(i);
				if (spShuriken->GetTraveledDistance() > 100.f)
				{
					spShuriken->SetThrown(false);
					SHPTR<ATransform> spShurikenTr = spShuriken->GetTransform();
					spShurikenTr->SetPos(spTransform->GetPos());
					spShuriken->ResetTraveledDistance();
				}
				else
				{
					spShuriken->ThrowShurikens(_dTimeDelta, m_arrThrowindDir[i]);
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