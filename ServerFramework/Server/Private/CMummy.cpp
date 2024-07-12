#include "ServerDefines.h"
#include "CMummy.h"
#include "ATransform.h"
#include "CMummyAnimController.h"
#include "CMobLayoutLoader.h"
#include "ANavigation.h"
#include "ACoreInstance.h"
#include "ACell.h"
#include "APathFinder.h"
#include "AAnimator.h"
#include "AAnimController.h"
#include "AAnimation.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer)
		: AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), 
		m_eMumyType{_MummyType}, m_isInitWakeUp{false}
	{
		SetMonsterType(TAG_CHAR::TAG_MUMMY);		
		UpdateFindRange(40.f,60.f);
		SetMoveSpeed(10);
		SetAttackRange(20.f);
		SetActive(false);
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin"));

		MOBDATA* pMoveData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		// Setting Animation 
		GetAnimController()->SetAnimation(pMoveData->strAnimName);
		GetTransform()->SetPos(pMoveData->mWorldMatrix.Get_Pos());
		GetTransform()->SetDirection(pMoveData->mWorldMatrix.Get_Look());
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		return true;
	}

	void CMummy::Tick(const _double& _dTimeDelta)
	{
		if (false == IsFoundPlayerFirstTime())
			return;

		__super::Tick(_dTimeDelta);
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		if (false == IsFoundPlayerFirstTime())
			return;

		FindPlayer(_spSession);

		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AAnimController> spAnimController = GetAnimController();
		SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
		SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<APathFinder> spPathFinder = spCoreInstance->GetPathFinder();
		_string strCurAnimName = spCurAnimation->GetAnimName();
		_bool Hit = false;

		VECTOR3 vRotate;
		VECTOR3 vPos;
		// Pos, Rotate를 만든다. 
		{
			Vector3 vRecvRotate = spTransform->GetRotationValue();
			Vector3 vRecvPos = spTransform->GetPos();
			PROTOFUNC::MakeVector3(&vRotate, vRecvRotate.x, vRecvRotate.y, vRecvRotate.z);
			PROTOFUNC::MakeVector3(&vPos, vRecvPos.x, vRecvPos.y, vRecvPos.z);
		}
		// wakeUp
		if (false == m_isInitWakeUp)
		{
			// Tick event
			spAnimController->SetInputTrigger("WAKEUP");
			SetMonsterState(MOB_FIRSTFIND_STATE);
			m_isInitWakeUp = true; 
		}

		if ("openLaying" == strCurAnimName || "openStanding" == strCurAnimName)
		{
			spAnimController->SetInputTrigger("TAUNT");
			SetMonsterState(MOB_FIND_STATE);
		}

		// Send Packet
		SC_MONSTERSTATEHAVEPOS scMonsterStateHavePos;
		{
			_int AnimIndex = spAnimController->GetCurAnimIndex();
			_double dTimeAcc = spCurAnimation->GetTimeAcc();
			PROTOFUNC::MakeScMonsterStateHavePos(&scMonsterStateHavePos, GetSessionID(), vPos, vRotate,
				dTimeAcc, AnimIndex, GetMonsterState());
			CombineProto<SC_MONSTERSTATEHAVEPOS>(GetCopyBuffer(), GetPacketHead(), scMonsterStateHavePos, TAG_SC_MONSTERSTATEHAVEMOVE);
			spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CMummy::Free()
	{
	}

}