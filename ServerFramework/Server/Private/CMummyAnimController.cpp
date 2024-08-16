#include "ServerDefines.h"
#include "CMummyAnimController.h"
#include "AAnimator.h"
#include "AAnimation.h"
#include "APawn.h"
#include "CMummy.h"

namespace Server {

	CMummyAnimController::CMummyAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_wpMummy{  }, m_isTauntMode{ false }, m_isAttackMode{ false },
		m_IdleTimer{ 1 }, m_IdleRandomValueChooseTimer { 3 },
		m_isStartlastHitTime{ false }, m_isLastAttackWasFirst{ true }, m_iRandomValue{ 0 }
	{
		m_wpMummy = static_shared_cast<CMummy>(_spPawn);
	}

	void CMummyAnimController::Tick(const _double& _dTimeDelta)
	{
		ClearState();
		__super::Tick(_dTimeDelta);

		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<> dis_patrol(0, 3);
		static std::uniform_int_distribution<> dis_attack(0, 1);

		SHPTR<CMummy> spMummy = m_wpMummy.lock();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spCurAnimation = GetCurAnimation();

		_string CurAnimName = spCurAnimation->GetAnimName();
		_bool isHit = spMummy->IsDamaged();
		_bool isFoundPlayer = spMummy->IsCurrentFindPlayer();
		_bool isDisableFoundPlayer = spMummy->IsCurrentNotFound();
		_bool isAtkPlayer = spMummy->IsCurrentAtkPlayer();
		_bool isFirstFound = spMummy->IsFoundPlayerFirstTime();
		_bool isDeadState = spMummy->IsDead();
		_bool isDeadStateEnable = spMummy->IsDeadStateEnable();
		MUMMYTYPE eMummyType = spMummy->GetMummyType();

		_bool isFirstFoundState = isFoundPlayer && (false == isFirstFound);
		_bool isFoundState = isFoundPlayer && isFirstFound;

		static const char* OPENLAYING = "openLaying";
		static const char* OPENSTANDING = "openStanding";
		static const char* TAUNTANIM = "taunt";
		static const char* GOTHITANIM = "gotHit";
		static const char* ATTACK1ANIM = "attack1";
		static const char* ATTACK2ANIM = "attack2";
		static const char* IDLE_ANIMNAME = "idle";

		static const char* WAKEUPORDER = "WAKEUP";
		static const char* IDLEORDER = "IDLE";
		static const char* WALKFORDER = "WALKF";
		static const char* TAUNTORDER = "TAUNT";
		static const char* GOTHITORDER = "GOTHIT";
		static const char* ATTACK1ORDER = "ATTACK01";
		static const char* ATTACK2ORDER = "ATTACK02";
		static const char* DEADORDER = "DEAD";

		if (false == isDeadState)
		{
			if (false == isFoundPlayer && false == isAtkPlayer && true == isDisableFoundPlayer)
			{
				m_isTauntMode = true;
			}

			if (true == isFoundPlayer && false == isFirstFound)
			{
				UpdateState(WAKEUPORDER, MOB_AWAKE_STATE);
				if (MUMMYTYPE::MUMMY_LAYING == eMummyType)
				{
					spAnimator->SetAnimation(OPENLAYING);
				}
				else
				{
					spAnimator->SetAnimation(OPENSTANDING);
				}

				spMummy->SetFoundPlayerFirstTime(true);
			}
			else if(false == isFoundPlayer && true == isFirstFound)
			{
				m_isAttackMode = false;

				if (true == m_IdleRandomValueChooseTimer.IsOver(_dTimeDelta))
				{
					m_iRandomValue = dis_patrol(gen);
					m_IdleRandomValueChooseTimer.ResetTimer();
				}

				if (IDLE_ANIMNAME == CurAnimName)
				{
					if (m_IdleTimer.IsOver(_dTimeDelta))
					{
						m_IdleTimer.ResetTimer();
					}
				}

				if (0 == m_IdleTimer.fTimer)
				{
					if (0 == m_iRandomValue)
						UpdateState(IDLEORDER, MOB_IDLE_STATE);
					else
						UpdateState(WALKFORDER, MOB_MOVE_STATE);
				}
			}
			else
			{
				if (false == isAtkPlayer)
				{
					UpdateState(WALKFORDER, MOB_MOVE_STATE);
				}
			}

			if (OPENLAYING == CurAnimName || OPENSTANDING == CurAnimName)
			{
				m_isTauntMode = true;
			}

			if (m_isTauntMode && false == isDisableFoundPlayer)
			{
				if (true == isFoundState && false == m_isAttackMode)
				{
					UpdateState(TAUNTORDER, MOB_TAUNT_STATE);
					if (TAUNTANIM == CurAnimName)
					{
						m_isAttackMode = true;
						m_isTauntMode = false;
					}
				}
			}

			if (true == isHit)
			{
				spAnimator->SetAnimation(GOTHITANIM);
				UpdateState(GOTHITORDER, MOB_HIT_STATE);
				spMummy->SetDamaged(false);
			}
			
			if (true == m_isAttackMode && false == isHit)
			{
				if (true == isAtkPlayer)
				{
					if (ATTACK1ANIM == CurAnimName)
						m_isLastAttackWasFirst = false;
					else if (ATTACK2ANIM == CurAnimName)
						m_isLastAttackWasFirst = true;

					if (true == m_isLastAttackWasFirst)
						UpdateState(ATTACK1ORDER, MOB_ATTACK_STATE);
					else
						UpdateState(ATTACK2ORDER, MOB_ATTACK_STATE);
				}
				else
				{
					UpdateState(WALKFORDER, MOB_MOVE_STATE);
				}
			}
		}

		// Handle death state
		if (true == isDeadState)
		{
			UpdateState(DEADORDER, MOB_DEATH_STATE);
		}
		else if (false == isFirstFound)
		{
			SetAnimState(MOB_IDLE_STATE);
		}

		spAnimator->TickEvent(spMummy.get(), GetInputTrigger(), _dTimeDelta);
	}

	void CMummyAnimController::Free()
	{
	}
}