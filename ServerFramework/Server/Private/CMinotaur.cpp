#include "ServerDefines.h"
#include "CMinotaur.h"
#include "ATransform.h"
#include "CMinotaurAnimController.h"
#include "AAnimController.h"
#include "ANavigation.h"
#include "ACell.h"
#include "ASession.h"
#include "AAnimController.h"
#include "AAnimator.h"
#include "ACoreInstance.h"
#include "AAnimation.h"
#include "ACollider.h"

namespace Server {

	CMinotaur::CMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_MINOTAUR);
		UpdateFindRange(100, 120);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 2500 });
	}

	_bool CMinotaur::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SHPTR<CMinotaurAnimController> spMino = Create<CMinotaurAnimController>(GetCoreInstance(), ThisShared<CMinotaur>(),
			"..\\..\\Resource\\Anim\\MinoTaur\\", "minotaur_FBX.bin", Matrix);

		SetAnimController(spMino);
		m_wpMinotaurAnimController = spMino;

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {3.f, 10.f, 3.f}, {0.f, 10.f, 0.f} });

		return __super::Start(_ReceiveDatas);
	}

	void CMinotaur::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		__super::Tick(_dTimeDelta);
	}

	void CMinotaur::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
//		__super::State(_spSession, _MonsterState);
	}

	void CMinotaur::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CMinotaur::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CMinotaur::TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession)
	{
		SHPTR<CMinotaurAnimController> spAnimController = m_wpMinotaurAnimController.lock();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<ANavigation> spNaviagation = GetNavigation();
		SHPTR<ACell> spCurrentMobCell = GetCurCell();
		SHPTR<ACell> spTargetCell = nullptr;
		Vector3 vCurrentMobPos = spTransform->GetPos();
		Vector3 vTargetPos;
		_string strCurAnimationName = spAnimation->GetAnimName();

		DEFINE_STATIC_CONSTCHAR(WALKFORWARD_ANIMNAME, "walk_forward2");
		DEFINE_STATIC_CONSTCHAR(RUN_ANIMNAME, "run");
		DEFINE_STATIC_CONSTCHAR(WALKBACK_ANIMNAME, "walk_back");
		DEFINE_STATIC_CONSTCHAR(HIT_ANIMNAME, "hit_1");

		if (nullptr != _spSession)
		{
			SHPTR<ATransform> spTargetTr = _spSession->GetTransform();
			spTargetCell = spNaviagation->FindCell(_spSession->GetCurCell());
			vTargetPos = spTargetTr->GetPos();
			SetTargetPos(vTargetPos);
		}

		SetPrevPosition(vCurrentMobPos);

		_bool isFoudnTargetState = IsCurrentFindPlayer();
		_bool isDeadState = IsDead();
	
		_int MobState = spAnimController->GetAnimState();

		spAnimController->Tick(_dTimeDelta);

		if (MOB_MOVE_STATE == MobState)
		{
			if (false == isDeadState)
			{
				if (true == isFoudnTargetState)
				{
					if (GetTimeAccumulator().IsOverLow())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}

					FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
					MoveAlongPath(_dTimeDelta);

					if (WALKFORWARD_ANIMNAME == strCurAnimationName)
					{
						spTransform->MoveBack(_dTimeDelta, RUNNING_SPEED);
					}
					else if (RUN_ANIMNAME == strCurAnimationName)
					{
						spTransform->MoveBack(_dTimeDelta, RUNNING_SPEED);
					}
					else if (WALKBACK_ANIMNAME == strCurAnimationName)
					{
						spTransform->MoveForward(_dTimeDelta, WALKING_SPEED);
					}
				}
				else
				{
					spTargetCell = spNaviagation->ChooseRandomNeighborCell(3);
					vTargetPos = spTargetCell->GetCenterPos();

					if (GetTimeAccumulator().IsOverHigh())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}

					FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
					MoveAlongPath(_dTimeDelta);
				}
			}
		}
		else if (MOB_ATTACK_STATE == MobState)
		{
			spTransform->LookAt(vTargetPos);
		}
		else if (MOB_HIT_STATE == MobState)
		{
			Vector3 vDirection = vTargetPos - vCurrentMobPos;
			spTransform->SetDirectionFixedUp(vDirection, _dTimeDelta, GetRotSpeed());
		}

		if (MOB_DEATH_STATE == MobState)
		{
			SetElapsedTime(GetElapsedTime() + (_dTimeDelta * GetDeadAnimSpeed()));
			if (GetElapsedTime() < GetDeadTimeArcOpenEnd())
			{
				spAnimator->TickAnimToTimeAccChangeTransform(spTransform, _dTimeDelta, GetElapsedTime());
			}
		}
		else if (MOB_IDLE_STATE == MobState)
		{
			spAnimator->TickAnimation(_dTimeDelta);
		}
		else
		{
			spAnimator->TickAnimChangeTransform(spTransform, _dTimeDelta);
			SetElapsedTime(0);
		}

		GetTimeAccumulator().PlusTime(_dTimeDelta);
		SetDamaged(false);
	}

	void CMinotaur::Free()
	{
	}

}