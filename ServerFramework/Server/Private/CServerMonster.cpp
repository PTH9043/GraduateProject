#include "ServerDefines.h"
#include "CServerMonster.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "AJobTimer.h"
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

	CServerMonster::CServerMonster(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer),
		m_iNextPathIndex{ 0 }, m_isPathFinding{ false }, m_TimeAccumulator{ 0.75, 2.5 },
		m_dDeadAnimSpeed{ 20 }, m_dElapsedTime{ 0 }, m_dDeadTimeArcOpenEnd{ 500 },
		m_fRotSpeed{ DirectX::XMConvertToRadians(90) }, m_fCollisionNuckbackSpeed{ 7.f },
		m_fApplySlidingMovementSpeed{ 20.f }, m_strRoomName{""}, m_DisableHitTimer{0.75f}
	{
		SetActive(false);
	}

	_bool CServerMonster::Start(const VOIDDATAS& _ReceiveDatas)
	{
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMobData->strAnimName);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		m_strRoomName = pMobData->strRoomName;
		BringCellIndextoPosition();
#else
		MOBSERVERDATA* pMobData = static_cast<MOBSERVERDATA*>(_ReceiveDatas[0]);
		GetAnimController()->SetAnimation(pMobData->iStartAnimIndex);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		m_strRoomName = pMobData->strRoomName;
		SetSessionID(pMobData->iMobID);
		SetOwnerMonsterSessionID(pMobData->iMobParentsID);
#endif
		SetPrevPosition(GetTransform()->GetPos());
		SetFoundPlayerFirstTime(false);
		UpdateSelfStateToPlayerDistance(false, false, false);


		//if (m_strRoomName == "Interior_Room_G")
		//{
		//	ActivePermanentDisable();
		//}
		//else if (m_strRoomName == "Interior_Hallway_E")
		//{
		//	ActivePermanentDisable();
		//}

		return __super::Start(_ReceiveDatas);
	}

	void CServerMonster::Tick(const _double& _dTimeDelta)
	{
		RunningDamagedToEnemyTimer(_dTimeDelta);
		DeadStateEnable(_dTimeDelta);
		RestrictPositionToNavi();
		UpdateColliderData();
		ResetTargetPlayer();
	}

	void CServerMonster::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		_bool isCurrentFindPlayer = IsCurrentFindPlayer();
		_bool isDamaged = IsDamaged();

		if (true == IsDeadStateEnable())
		{
			// 영구적 비활성화
			ActivePermanentDisable();
		}

		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();

		Vector3 vPos = spTransform->GetPos();
		Vector3 vRotate = spTransform->GetRotationValue();

		VECTOR3 vSendPos, vSendRotate;

		if (true == IsFoundPlayerFirstTime())
		{
			{
				PROTOFUNC::MakeVector3(&vSendPos, vPos.x, vPos.y, vPos.z);
				PROTOFUNC::MakeVector3(&vSendRotate, vRotate.x, vRotate.y, vRotate.z);
			}

			_int AnimState = spAnimController->GetAnimState();
			_double dTimeAcc = spCurAnimation->GetTimeAcc();
			_int AnimIndex = spAnimator->GetCurAnimIndex();

			MOBSTATE monsterState;
			PROTOFUNC::MakeMobState(&monsterState, GetSessionID(), vSendPos, vSendRotate,
				AnimState, AnimIndex, false, isCurrentFindPlayer, isDamaged, dTimeAcc);
			CombineProto<MOBSTATE>(GetCopyBuffer(), GetPacketHead(), monsterState, TAG_SC_MONSTERSTATE);
			_spSession->SendData(GetCopyBufferPointer(), GetPacketHead());
			if (true == IsDead() || true == IsDeadStateEnable())
			{
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			}
			else
			{
				_spSession->SendData(GetCopyBufferPointer(), GetPacketHead());
			}
		}
		else
		{
			if (true == IsDead() || true == IsDeadStateEnable())
			{
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			}
		}
	}

	void CServerMonster::ProcessPacket(_int _type, void* _pData){}

	void CServerMonster::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		//AGameObject* pGameObject = _pGameObject;
		//RETURN_CHECK(nullptr == pGameObject, ;);
		//RETURN_CHECK(false == IsCurrentAtkPlayer(), ;);
		//SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		//SESSIONTYPE EnemySessionType = pGameObject->GetSessionType();
		//SHPTR<ATransform> spSelfTr = GetTransform();
		//SHPTR<ATransform> spTargetTr = pGameObject->GetTransform();

		//Vector3 vSelfPos = spSelfTr->GetPos();
		//Vector3 vTargetPos = spTargetTr->GetPos();

		//Vector3 vReverseTargetDirection = vSelfPos - vTargetPos;
		//vReverseTargetDirection.Normalize();

		//const auto& SelfCollisionCollider = GetColliderContainer();
		//const auto& TargetCollisionCollider = pGameObject->GetColliderContainer();

		//if (SESSIONTYPE::PLAYER == EnemySessionType || SESSIONTYPE::MONSTER == EnemySessionType)
		//{
		//	for (auto& SelfCollision : SelfCollisionCollider)
		//	{
		//		for (auto& TargetCollision : TargetCollisionCollider)
		//		{
		//			if (COLLIDERTYPE::COLLIDER_MAIN == TargetCollision.first)
		//			{
		//				if (SelfCollision.second->IsCollision(TargetCollision.second))
		//				{
		//					spSelfTr->TranslateDir(vReverseTargetDirection, _dTimeDelta, GetCollisionNuckbackSpeed());
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
		//else if (SESSIONTYPE::OBJECT == EnemySessionType)
		//{
		//	for (auto& SelfCollision : SelfCollisionCollider)
		//	{
		//		for (auto& TargetCollision : TargetCollisionCollider)
		//		{
		//			if (COLLIDERTYPE::COLLIDER_MAIN == TargetCollision.first)
		//			{
		//				if (SelfCollision.second->IsCollision(TargetCollision.second))
		//				{
		//					Vector3 vColliderNormal = SelfCollision.second->GetCollisionNormal(TargetCollision.second);
		//					spSelfTr->ApplySlidingMovement(GetPrevPosition(), vColliderNormal,
		//						GetApplySlidingMovementSpeed(), _dTimeDelta);
		//					break;
		//				}
		//			}
		//		}
		//	}
		//}
	}

	void CServerMonster::RunningPermanentDisableSituation()
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();

		Vector3 vPos = spTransform->GetPos();
		Vector3 vRotate = spTransform->GetRotationValue();

		VECTOR3 vSendPos, vSendRotate;
		{
			PROTOFUNC::MakeVector3(&vSendPos, vPos.x, vPos.y, vPos.z);
			PROTOFUNC::MakeVector3(&vSendRotate, vRotate.x, vRotate.y, vRotate.z);
		}

		_int AnimState = spAnimController->GetAnimState();
		_double dTimeAcc = spCurAnimation->GetTimeAcc();
		_int AnimIndex = spAnimator->GetCurAnimIndex();

		MOBSTATE monsterState;
		PROTOFUNC::MakeMobState(&monsterState, GetSessionID(), vSendPos, vSendRotate,
			AnimState, AnimIndex, true, false, false, dTimeAcc);
		CombineProto<MOBSTATE>(GetCopyBuffer(), GetPacketHead(), monsterState, TAG_SC_MONSTERSTATE);
		spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
	}

	void CServerMonster::TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession)
	{
		SHPTR<AAnimController> spAnimController = GetAnimController();
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

		if (nullptr != _spSession)
		{
			SHPTR<ATransform> spTargetTr = _spSession->GetTransform();
			spTargetCell = spNaviagation->FindCell(_spSession->GetCurCell());
			vTargetPos = spTargetTr->GetPos();
			SetTargetPos(vTargetPos);
			iCurrentPlayerState = _spSession->GetCurrentAnimState();
		}
		m_vTargetToDir = vTargetPos - vCurrentMobPos;
		SetDistanceToPlayer(m_vTargetToDir.Length());
		m_vTargetToDir.Normalize();
		SetPrevPosition(vCurrentMobPos);
		SetPlayerToDot(OtherCharacterDirToLookConverter(vTargetPos));

		_bool isFirstFoundState = IsFoundPlayerFirstTime();
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
					if (m_TimeAccumulator.IsOverLow())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
				}
				else
				{
					spTargetCell = spNaviagation->ChooseRandomNeighborCell(3);
					vTargetPos = spTargetCell->GetCenterPos();

					if (m_TimeAccumulator.IsOverHigh())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
				}
				SetTargetPos(vTargetPos);
				FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
				MoveAlongPath(_dTimeDelta);
			}
		}

		spAnimator->TickAnimChangeTransform(spTransform, _dTimeDelta);
		m_TimeAccumulator.PlusTime(_dTimeDelta);
		DisableDamaged(iCurrentPlayerState, _dTimeDelta);
	}

	void CServerMonster::MoveAlongPath(const _double& _dTimeDelta)
	{
		size_t pathSize = m_AstarPath.size();

		_int CurrentPathIndex = m_iNextPathIndex;
		_float RotSpeed = m_fRotSpeed;

		SHPTR<ATransform> spTransform = GetTransform();

		Vector3 vCurrentPos = spTransform->GetPos();
		Vector3 vTargetPos;
		Vector3 Direction;

		if (pathSize > 0 && CurrentPathIndex < pathSize)
		{
			vTargetPos = m_AstarPath[CurrentPathIndex];

			Direction = vTargetPos - vCurrentPos;
			_float fDistance = Direction.Length();
			if (fDistance > GetDistanceToPlayer())
			{
				vTargetPos = GetTargetPos();
				m_iNextPathIndex.fetch_add(1);
			}
			else
			{
				if (fDistance <= 1.f)
				{
					m_iNextPathIndex.fetch_add(1);
				}
			}
		}
		else
		{
			vTargetPos = GetTargetPos();
			Direction = vTargetPos - vCurrentPos;
		}

		SetDirectionFixedUp(_dTimeDelta, GetPlayerToDot(), 10, vTargetPos);
	}

	void CServerMonster::StartFindPath(SHPTR<ANavigation> _spNavigation, SHPTR<ACell> _spSelfCell, SHPTR<ACell> _spTargetCell, 
		const Vector3 _vSelfPos, const Vector3 _vTargetPos)
	{
		if (false == m_isPathFinding)
		{
			m_PathFindState = _spNavigation->StartPathFinding(_vSelfPos, _vTargetPos, _spSelfCell, _spTargetCell);
			m_isPathFinding = true;
			m_TimeAccumulator.ResetTimer();
		}
	}

	void CServerMonster::FindingPath(SHPTR<ANavigation> _spNavigation, const Vector3 _vSelfPos, const Vector3 _vTargetPos)
	{
		if (true == m_isPathFinding)
		{
			if (true == _spNavigation->StepPathFinding(m_PathFindState))
			{
				m_isPathFinding = false;
				if (true == m_PathFindState.isPathFound)
				{
					m_AstarPath = _spNavigation->OptimizePath(m_PathFindState.path, _vSelfPos, _vTargetPos);
					m_iNextPathIndex = 0;
				}
			}
		}
	}

	void CServerMonster::DisableDamaged(_int _iPlayerState, const _double& _dTimeDelta)
	{
		RETURN_CHECK(false == IsDamaged(), ;);

		if (PLAYER_ANIM_ATTACK != _iPlayerState || m_DisableHitTimer.IsOver(_dTimeDelta))
		{
			SetDamaged(false);
			m_DisableHitTimer.ResetTimer();
		}
	}

	void CServerMonster::Free()
	{
	}

}