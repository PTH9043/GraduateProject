#include "ServerDefines.h"
#include "CAnubis.h"
#include "ATransform.h"
#include "CAnubisAnimController.h"
#include "ACollider.h"
#include "ASession.h"
#include "AAnimator.h"
#include "AAnimation.h"
#include "ATransform.h"
#include "ACoreInstance.h"
#include "ANavigation.h"
#include "ACell.h"


namespace Server
{
	CAnubis::CAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), 
		m_vOriginPos{}, m_vOriginToDir{}, m_fOriginPosToDot{0}, m_isShield{false}, m_isFireAttack{false}
	{
		SetMonsterType(TAG_CHAR::TAG_ANUBIS);
		UpdateFindRange(70, 80);
		SetMoveSpeed(50);
		SetAttackRange(10.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1 });
	}
	_bool CAnubis::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CAnubisAnimController>(GetCoreInstance(), ThisShared<CAnubis>(),
			"..\\..\\Resource\\Anim\\Anubis\\", "Anubis_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {0.f, 10.f, 0.f}, {1.f, 8.f, 1.f} });

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAGICSPHERE, ACollider::TYPE_SPHERE,
			COLLIDERDESC{ {8.f, 8.f, 8.f}, {0.f, 5.f, 0.f} });

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAGICCIRCLE, ACollider::TYPE_OBB,
			COLLIDERDESC{ {1.f, 1.f, 1.f}, {0.f, 0.5f, 0.f} });
		__super::Start(_ReceiveDatas);

		SHPTR<ATransform> spTransform = GetTransform();
		spTransform->SetPos(BringCellIndextoPosition());
		m_vOriginPos = spTransform->GetPos();
		return  true;
	}

	void CAnubis::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		__super::Tick(_dTimeDelta);
	}

	void CAnubis::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
 		__super::State(_spSession, _MonsterState);
	}

	void CAnubis::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CAnubis::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	_bool CAnubis::ComputeCheckYPlayerPos(const _float _fMin, const _float _fMax)
	{
		SHPTR<ATransform> spTransform = GetTransform();
		Vector3 vPos = spTransform->GetPos();
		for (auto& iter : GetPlayerPositionContainerRefP())
		{
			_float yValue = vPos.y - iter.second.y;
			if (yValue >= _fMin && yValue <= _fMax)
				return true;
		}
		return false;
	}

	void CAnubis::TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession)
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
			iCurrentPlayerState = _spSession->GetCurrentAnimState();
			SetTargetPos(vTargetPos);
		}
		Vector3 vDir = vTargetPos - vCurrentMobPos;
		SetDistanceToPlayer(vDir.Length());
		SetPrevPosition(vCurrentMobPos);
		SetPlayerToDot(OtherCharacterDirToLookConverter(vTargetPos));
		m_vOriginToDir = m_vOriginPos - vCurrentMobPos;
		m_vOriginToDir.Normalize();
		m_fOriginPosToDot = OtherCharacterDirToLookConverter(m_vOriginPos);
		vDir.Normalize();
		SetTargetToDir(vDir);

		DEFINE_STATIC_CONSTCHAR(WALK_ANIM, "Walk");
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
					if (GetTimeAccumulatorRefP(REF_RETURN).IsOverLow())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
					FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
					MoveAlongPath(_dTimeDelta);
				}
				else
				{
					spTargetCell = spNaviagation->ChooseRandomNeighborCell(3);
					vTargetPos = spTargetCell->GetCenterPos();

					if (GetTimeAccumulatorRefP(REF_RETURN).IsOverHigh())
					{
						StartFindPath(spNaviagation, spCurrentMobCell, spTargetCell, vCurrentMobPos, vTargetPos);
					}
					SetTargetPos(vTargetPos);
					FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
					MoveAlongPath(_dTimeDelta);
				}
			}
		}
		else if (MOB_ATTACK_STATE == MobState)
		{
			SetDirectionFixedUp(_dTimeDelta, GetPlayerToDot(), 10, vTargetPos);
		}

		if (strCurAnimationName == WALK_ANIM)
		{
			static constexpr _float MOVESPEED{ 15 };
			spTransform->MoveForward(_dTimeDelta, MOVESPEED);
		}

		if (MOB_IDLE_STATE != MobState)
		{
			SetElapsedTime(0);
		}

		spAnimator->TickAnimation(_dTimeDelta);
		GetTimeAccumulatorRefP(REF_RETURN).PlusTime(_dTimeDelta);
		DisableDamaged(iCurrentPlayerState, _dTimeDelta);
	}

	void CAnubis::Free()
	{
	}

}