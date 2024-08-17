#include "ServerDefines.h"
#include "CAnubisAnimController.h"
#include "CAnubis.h"
#include "AAnimation.h"
#include "AAnimator.h"
#include "ATransform.h"

namespace Server {

	CAnubisAnimController::CAnubisAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_isAttackMode{false},	m_isAttackStart{false},	m_isTauntMode{false},
	m_ShieldCoolTime{11}, m_HitCooldownTimer{1},m_iHitCount{0}, m_iRandomValueforAttack{0}
	{
		m_wpAnubis = static_shared_cast<CAnubis>(_spPawn);
		m_HitCooldownTimer.fTimer = m_HitCooldownTimer.fStandardTime;
		m_ShieldCoolTime.fTimer = m_ShieldCoolTime.fStandardTime;
	}

	void CAnubisAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_attack(0, 1);
		static std::uniform_int_distribution<> dis_hit(0, 2);

		static constexpr _float ATTACKRANGE_LONG{ 40 };
		static constexpr _float ATTACKRANGE_CLOSE{ 10 };

		SHPTR<CAnubis>	spAnubis = m_wpAnubis.lock();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = spAnubis->GetTransform();

		_string CurAnimName = spAnimation->GetAnimName();
		_bool isHit = spAnubis->IsDamaged();
		_bool isFoundPlayer = spAnubis->IsCurrentFindPlayer();
		_bool isDisableFoundPlayer = spAnubis->IsCurrentNotFound();
		_bool isAtkPlayer = spAnubis->IsCurrentAtkPlayer();
		_bool isFirstFound = spAnubis->IsFoundPlayerFirstTime();
		_bool isDeadState = spAnubis->IsDead();
		_float fTargetToDistance = spAnubis->GetDistanceToPlayer();
		_float fProgressRate = spAnimation->GetAnimationProgressRate();
		_float fPlayerToDot = spAnubis->GetPlayerToDot();
		Vector3 vTargetToDir = spAnubis->GetTargetToDir();
		Vector3 vSelfPos = spTransform->GetPos();
		Vector3 vOriginPos = spAnubis->GetOriginPos();
		_float fDot = spAnubis->GetPlayerToDot();
		_float fOriginDot = spAnubis->GetPlayerToDot();

		DEFINE_STATIC_CONSTCHAR(IDLE_ORDER, "IDLE");
		DEFINE_STATIC_CONSTCHAR(WALK_ORDER, "WALK");
		DEFINE_STATIC_CONSTCHAR(TOATTACKIDLE_ORDER, "TOATTACKIDLE");
		DEFINE_STATIC_CONSTCHAR(TOGUARDIDLE_ORDER, "TOGUARDIDLE");
		DEFINE_STATIC_CONSTCHAR(ATTACKIDLE_ORDER, "ATTACKIDLE");
		DEFINE_STATIC_CONSTCHAR(ATTACK3_ORDER, "ATTACK3");
		DEFINE_STATIC_CONSTCHAR(ATTACK1_ORDER, "ATTACK1");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ORDER, "ATTACK4");
		DEFINE_STATIC_CONSTCHAR(CAST_ORDER, "CAST");

		DEFINE_STATIC_CONSTCHAR(ATKIDLETOGUARDIDLE_ANIM, "Attack Idle to Guard Idle");
		DEFINE_STATIC_CONSTCHAR(GUARDIDLETOATKIDLE_ANIM, "Guard Idle to Attack Idle");
		DEFINE_STATIC_CONSTCHAR(WALK_ANIM, "Walk");
		DEFINE_STATIC_CONSTCHAR(GUARDIDLE_ANIM, "Guard Idle");
		DEFINE_STATIC_CONSTCHAR(HITREACTION_ANIM, "Hit Reaction");
		DEFINE_STATIC_CONSTCHAR(ATTACKIDLE_ANIM, "Attack Idle");
		DEFINE_STATIC_CONSTCHAR(ATTACK3_ANIM, "Attack3");
		DEFINE_STATIC_CONSTCHAR(CAST_ANIM, "Cast");

		if (false == isDeadState)
		{
			 if (true == isFoundPlayer)
			{
				 if (false == isFirstFound)
				 {
					 spAnimator->SetAnimation(GUARDIDLETOATKIDLE_ANIM);
					 UpdateState(TOATTACKIDLE_ORDER, MOB_AWAKE_STATE);
					 spAnubis->SetFoundPlayerFirstTime(true);
				 }
				 else
				 {
					 if (GUARDIDLETOATKIDLE_ANIM == CurAnimName || WALK_ANIM == CurAnimName)
					 {
						 m_isAttackMode = true;
					 }
				 }
			}
			 else 
			 {
				 m_isAttackMode = false;
				 m_isAttackStart = false;

				 if (true == isFirstFound)
				 {
					 static constexpr _float JUDGE_DISTANCE{ 2.f };

					 if (Vector3::Distance(vSelfPos, vOriginPos) >= JUDGE_DISTANCE)
					 {
						 UpdateState(WALK_ORDER, MOB_MOVE_STATE);
					 }
					 else
					 {
						 UpdateState(TOGUARDIDLE_ORDER, MOB_AWAKE_STATE);
						 spAnimator->SetAnimation(ATKIDLETOGUARDIDLE_ANIM);
						 spAnubis->SetFoundPlayerFirstTime(false);
						 m_ShieldCoolTime.fTimer = m_ShieldCoolTime.fStandardTime;
					 }
				 }
				 else
				 {
					 if (ATKIDLETOGUARDIDLE_ANIM == CurAnimName)
					 {
						 if (fProgressRate >= 0.85f)
						 {
							 UpdateState(TOGUARDIDLE_ORDER, MOB_IDLE_STATE);
						 }
					 }
					 else if(GUARDIDLE_ANIM == CurAnimName)
					 {
						 spAnubis->SetDirectionFixedUp(_dTimeDelta, fOriginDot, 10, vOriginPos);
					 }
				 }
			 }

			 if (true == isHit && true == m_HitCooldownTimer.IsOver(_dTimeDelta))
			 {
				 if (m_iHitCount < 3)
				 {
					 spAnimator->SetAnimation(HITREACTION_ANIM);
					 SetAnimState(MOB_HIT_STATE);
					 ++m_iHitCount;
				 }
				 else
				 {
					 m_iHitCount = 0;
					 m_HitCooldownTimer.ResetTimer();
				 }
			 }
			 else if (true == m_isAttackMode && false == isHit)
			 {
				 UpdateState(ATTACKIDLE_ORDER, MOB_IDLE_STATE);

				 if (false == m_isAttackStart)
				 {
					 spAnimator->SetAnimation(ATTACKIDLE_ANIM);
					 SetAnimState(MOB_IDLE_STATE);
					 m_isAttackStart = true;
				 }

				 if (true == m_isAttackStart)
				 {
					 if (m_ShieldCoolTime.IsOver(_dTimeDelta))
					 {
						 UpdateState(ATTACK3_ORDER, MOB_ATTACK_STATE);
					 }
					 else
					 {
						 if (fTargetToDistance >= ATTACKRANGE_LONG)
						 {
							 UpdateState(WALK_ORDER, MOB_MOVE_STATE);
						 }
						 else if (fTargetToDistance < ATTACKRANGE_LONG && fTargetToDistance >= ATTACKRANGE_CLOSE)
						 {
							 if(spAnubis->ComputeCheckYPlayerPos(-0.2f, 0.2f))
								 UpdateState(CAST_ORDER, MOB_ATTACK_STATE);
							 else
								 UpdateState(WALK_ORDER, MOB_MOVE_STATE);
						 }
						 else if (fTargetToDistance < ATTACKRANGE_CLOSE)
						 {
							 if (fProgressRate >= 0.85)
								 m_iRandomValueforAttack = dis_attack(gen);

							 if (m_iRandomValueforAttack == 0)
								 UpdateState(ATTACK1_ORDER, MOB_ATTACK_STATE);
							 else
								 UpdateState(ATTACK4_ORDER, MOB_ATTACK_STATE);
						 }
					 }
				 }
			 }
		}
		else
		{
			UpdateState("DEATH", MOB_DEATH_STATE);
		}

		if (CurAnimName == ATTACK3_ANIM)
		{
			if (fProgressRate >= 0.65f && fProgressRate <= 0.66f)
			{
				m_ShieldCoolTime.ResetTimer();
			}
		}

			// Tick Event
		spAnimator->TickEvent(spAnubis.get(), GetInputTrigger(), _dTimeDelta);
	}

	void CAnubisAnimController::Free()
	{
	}
}