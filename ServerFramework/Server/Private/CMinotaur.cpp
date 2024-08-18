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
		SetAttackRange(10.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1 });
	}

	_bool CMinotaur::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 	Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
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
	__super::State(_spSession, _MonsterState);
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
		_int iCurrentPlayerState{ 0 };

		DEFINE_STATIC_CONSTCHAR(WALKFORWARD_ANIMNAME, "walk_forward2");
		DEFINE_STATIC_CONSTCHAR(RUN_ANIMNAME, "run");
		DEFINE_STATIC_CONSTCHAR(WALKBACK_ANIMNAME, "walk_back");
		DEFINE_STATIC_CONSTCHAR(HIT_ANIMNAME, "hit_1");

		if (nullptr != _spSession)
		{
			SHPTR<ATransform> spTargetTr = _spSession->GetTransform();
			spTargetCell = spNaviagation->FindCell(_spSession->GetCurCell());
			vTargetPos = spTargetTr->GetPos();
			iCurrentPlayerState = _spSession->GetCurrentAnimState();
			SetTargetPos(vTargetPos);
		}

		SetDistanceToPlayer(Vector3::Distance(vTargetPos , vCurrentMobPos));
		SetPrevPosition(vCurrentMobPos);
		SetPlayerToDot(OtherCharacterDirToLookConverter(vTargetPos));

		_bool isFoudnTargetState = IsCurrentFindPlayer();
		_bool isDeadState = IsDead();
	
		_int MobState = spAnimController->GetAnimState();

		spAnimController->Tick(_dTimeDelta);

		if (MOB_MOVE_STATE == MobState || CMinotaurAnimController::MOB_RUN_STATE == MobState || 
			CMinotaurAnimController::MOB_BACK_STATE == MobState)
		{
			if (false == isDeadState)
			{
				if (true == isFoudnTargetState)
				{
					if (GetTimeAccumulatorRefP().IsOverLow())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
				}
				else
				{
					spTargetCell = spNaviagation->ChooseRandomNeighborCell(3);
					vTargetPos = spTargetCell->GetCenterPos();

					if (GetTimeAccumulatorRefP().IsOverHigh())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}

					SetTargetPos(vTargetPos);
				}

				FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
				MoveAlongPath(_dTimeDelta);
			}
		}

		if (WALKFORWARD_ANIMNAME == strCurAnimationName)
		{
			spTransform->MoveForward(_dTimeDelta, WALKING_SPEED);
		}
		else if (RUN_ANIMNAME == strCurAnimationName)
		{
			spTransform->MoveForward(_dTimeDelta, RUNNING_SPEED);
		}
		else if(WALKBACK_ANIMNAME == strCurAnimationName)
		{
			spTransform->MoveBack(_dTimeDelta, WALKING_SPEED);
		}


		spAnimator->TickAnimation(_dTimeDelta);
		GetTimeAccumulatorRefP().PlusTime(_dTimeDelta);
		DisableDamaged(iCurrentPlayerState, _dTimeDelta);
	}

	void CMinotaur::MoveAlongPath(const _double& _dTimeDelta)
	{
		__super::MoveAlongPath(_dTimeDelta);
	}

	void CMinotaur::Free()
	{
	}

}