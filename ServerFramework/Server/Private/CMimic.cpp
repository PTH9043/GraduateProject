#include "ServerDefines.h"
#include "CMimic.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "CMimicAnimController.h"
#include "ANavigation.h"
#include "ACell.h"
#include "ASession.h"
#include "AAnimController.h"
#include "AAnimator.h"
#include "ACoreInstance.h"
#include "AAnimation.h"
#include "ACollider.h"

namespace Server {

	CMimic::CMimic(OBJCON_CONSTRUCTOR, SESSIONID _ID,
		SHPTR<AJobTimer> _spMonsterJobTimer) : 
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer),
		m_isPressKeyEnable{false}, m_isOpeningState{false}
	{
		SetMonsterType(TAG_CHAR::TAG_MIMIC);
		UpdateFindRange(10, 120);
		SetMoveSpeed(20);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1 });
	}

	_bool CMimic::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CMimicAnimController>(GetCoreInstance(), ThisShared<CMimic>(),
			"..\\..\\Resource\\Anim\\Mimic\\", "Mimic_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {3.f, 7.f, 3.f}, {0.f, 3.f, 0.f} });

		return __super::Start(_ReceiveDatas);
	}

	void CMimic::Tick(const _double& _dTimeDelta)
	{
		RETURN_CHECK(false == m_isPressKeyEnable, ;);
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		__super::Tick(_dTimeDelta);
	}

	void CMimic::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
    	__super::State(_spSession, _MonsterState);

		if (IsCurrentFindPlayer())
		{
			if (false == m_isPressKeyEnable)
			{
				SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
				_int KeyState = _spSession->GetKeyState();
				_int enable = KeyState == KEYBOARD_F ? 1 : 0;
				SC_STATICOBJFIND scMonsterFind;
				PROTOFUNC::MakeScStaticObjFind(&scMonsterFind, GetSessionID(), enable);
				CombineProto<SC_STATICOBJFIND>(GetCopyBuffer(), GetPacketHead(), scMonsterFind, TAG_SC_STATICOBJFIND);
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				if (1 == enable)
				{
					m_isPressKeyEnable = true;
				}
			}
		}
	}

	void CMimic::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CMimic::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CMimic::TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession)
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

		DEFINE_STATIC_CONSTCHAR(HITNAME, "Hit");

		if (nullptr != _spSession)
		{
			SHPTR<ATransform> spTargetTr = _spSession->GetTransform();
			spTargetCell = spNaviagation->FindCell(_spSession->GetCurCell());
			vTargetPos = spTargetTr->GetPos();

			SetTargetPos(vTargetPos);
		}
		SetDistanceToPlayer(Vector3::Distance(vTargetPos, vCurrentMobPos));
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
				FindingPath(spNaviagation, vCurrentMobPos, vTargetPos);
				MoveAlongPath(_dTimeDelta);
				GetTransform()->MoveForward( _dTimeDelta, GetMoveSpeed());
			}
		}
		else if (MOB_ATTACK_STATE == MobState)
		{
			SetDirectionFixedUp(_dTimeDelta, GetPlayerToDot(), 10, vTargetPos);
		}
		else if (MOB_HIT_STATE == MobState)
		{
			if (HITNAME == spAnimation->GetAnimName())
			{
				spTransform->MoveBack(_dTimeDelta, NUCKBACK_SPEED);
			}
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

		GetTimeAccumulatorRefP().PlusTime(_dTimeDelta);
		SetDamaged(false);
	}

	void CMimic::Free()
	{
	}
}
