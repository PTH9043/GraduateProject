#include "ServerDefines.h"
#include "CHarlequin.h"
#include "ATransform.h"
#include "CHarlequinAnimController.h"
#include "ACollider.h"
#include "ASession.h"
#include "AAnimator.h"
#include "AAnimation.h"
#include "ATransform.h"
#include "CShurikenThrowing.h"
#include "ACoreInstance.h"
#include "ANavigation.h"
#include "ACell.h"

namespace Server {

	CHarlequin::CHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_HARLEQUINN);
		UpdateFindRange(80, 150);
		SetMoveSpeed(5);
		SetAttackRange(60.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1500 });

		SESSIONID id = 0;
		for (auto& iter : m_ShurikenContainer)
		{
			iter = Create<CShurikenThrowing>(GetCoreInstance(), id++);
		}
	}

	_bool CHarlequin::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 	Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CHarlequinAnimController>(GetCoreInstance(), ThisShared<CHarlequin>(),
			"..\\..\\Resource\\Anim\\Harlequin\\", "Harlequin1_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {3.f, 10.f, 3.f}, {0.f, 10.f, 0.f} });

		__super::Start(_ReceiveDatas);

		SHPTR<ATransform> spTransform = GetTransform();
		_float4x4 mWorldMatrix = spTransform->GetWorldMatrix();

		for (auto& iter : m_ShurikenContainer)
		{
			iter->Start({ &mWorldMatrix });
		}
		return true;
	}

	void CHarlequin::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		RunningDamagedToEnemyTimer(_dTimeDelta);
		DeadStateEnable(_dTimeDelta);
		UpdateColliderData();
		ResetTargetPlayer();
	}

	void CHarlequin::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		__super::State(_spSession, _MonsterState);
	}

	void CHarlequin::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CHarlequin::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	SHPTR<CShurikenThrowing> CHarlequin::GetShurikenThrowing(_int _iIndex)
	{
		RETURN_CHECK(_iIndex >= THROWING_NUM, nullptr);
		return m_ShurikenContainer[_iIndex];
	}

	void CHarlequin::TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession)
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

		DEFINE_STATIC_CONSTCHAR(HITNAME, "Get Hit");
		DEFINE_STATIC_CONSTCHAR(JUMPFORWARD, "Jump Forward");
		DEFINE_STATIC_CONSTCHAR(ATTACK3, "Attack 4");
		DEFINE_STATIC_CONSTCHAR(ATTACK4, "Attack 3");

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
		vDir.Normalize();
		SetTargetToDir(vDir);

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
				}
				SetTargetPos(vTargetPos);
				FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
				MoveAlongPath(_dTimeDelta);
			}
		}

		if (strCurAnimationName == HITNAME)
		{
			spTransform->SetDirectionFixedUp(GetTargetToDir(), _dTimeDelta, GetRotSpeed());
		}

		spAnimator->TickAnimChangeTransform(spTransform, _dTimeDelta);
		GetTimeAccumulatorRefP().PlusTime(_dTimeDelta);
		DisableDamaged(iCurrentPlayerState, _dTimeDelta);

		if (strCurAnimationName != JUMPFORWARD && strCurAnimationName != ATTACK4 &&
			strCurAnimationName != ATTACK3)
		{
			RestrictPositionToNavi();
		}
		else
		{
			Vector3 vPosition = spTransform->GetPos();
			spNaviagation->ComputeHeight(REF_OUT vPosition);
			SHPTR<ACell> spCell = nullptr;
			if (false == spNaviagation->IsMove(vPosition, REF_OUT spCell))
			{
				Vector3 vChangePos = spTransform->GetPos();
				vCurrentMobPos.y = vChangePos.y;
				spTransform->SetPos(vCurrentMobPos);
				m_isDontMove = true;
			}
			else
			{
				spTransform->SetPos(vPosition);
			}
		}
	}

	void CHarlequin::MoveAlongPath(const _double& _dTimeDelta)
	{
		__super::MoveAlongPath(_dTimeDelta);
	}

	void CHarlequin::LastBehavior()
	{
		__super::LastBehavior();
	}

	void CHarlequin::Free()
	{
	}

}