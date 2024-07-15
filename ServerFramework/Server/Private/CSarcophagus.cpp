#include "ServerDefines.h"
#include "CSarcophagus.h"
#include "CMummy.h"
#include "ATransform.h"
#include "AAnimator.h"
#include "CSacrophagusAnimController.h"
#include "ACoreInstance.h"
#include "AAnimation.h"
#include "CMobLayoutLoader.h"
#include "CMobLayOutSaver.h"

namespace Server
{
	CSarcophagus::CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType, 
		SHPTR<AJobTimer> _spMonsterJobTimer) :
		AMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), m_eSarcophagusType{_eSarcophagusType}, m_isInitStart{false}
	{
		if (SARCO_LAYING == m_eSarcophagusType)
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_STANDING);
		}

		UpdateFindRange(40.f, 80.f);
	}

	_bool CSarcophagus::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		ASSERT_CRASH(_ReceiveDatas.size() >= 0);
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMoveData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		SESSIONID* pSessionID = static_cast<SESSIONID*>(_ReceiveDatas[1]);

		SHPTR<CSacrophagusAnimController> spSacrophagusAnimController = nullptr;

		SHPTR<CMummy> spMummy;
		switch (m_eSarcophagusType)
		{
		case SARCO_LAYING:
		{
			spSacrophagusAnimController =	Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(), 
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusLaying_FBX.bin", Matrix);
			spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_LAYING, GetMonsterJobTimer());
		}
		break;
		case SARCO_STANDING:
		{
			spSacrophagusAnimController = Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(),
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusStanding_FBX.bin", Matrix);
			spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_STANDING, GetMonsterJobTimer());
		}
		break;
		}
		SetAnimController(spSacrophagusAnimController);
		spMummy->Start({ _ReceiveDatas[0] });
		GetTransform()->SetNewWorldMtx(pMoveData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		GetCoreInstance()->InsertMobObject(spMummy->GetSessionID(), spMummy);

		if (SARCO_LAYING == m_eSarcophagusType)
			spMummy->GetTransform()->TranslateDir((spMummy->GetTransform()->GetLook()), 1, 12);
#else
		SHPTR<CSacrophagusAnimController> spSacrophagusAnimController = Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(),
			"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusLaying_FBX.bin", Matrix);
		SetAnimController(spSacrophagusAnimController);

		MOBSERVERDATA* pMobData = static_cast<MOBSERVERDATA*>(_ReceiveDatas[0]);
		GetAnimController()->SetAnimation(pMobData->iStartAnimIndex);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		SetSessionID(pMobData->iMobID);
#endif
		return S_OK;
	}

	void CSarcophagus::Tick(const _double& _dTimeDelta)
	{
		__super::Tick(_dTimeDelta);

		if (true == IsCurrentFindPlayer())
		{
			if (false == m_isInitStart)
			{
				_int AnimIndex = GetAnimController()->GetCurAnimIndex();
				_double dDuration = GetAnimController()->GetElapsedTime();

				SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
				SHPTR<AAnimation> spCurAnimation = GetAnimController()->GetCurAnimation();
				SHPTR<ATransform> spTransform = GetTransform();

				SC_MONSTERSTATE scMonsterState;
				PROTOFUNC::MakeScMonsterState(&scMonsterState, GetSessionID(), dDuration, AnimIndex, TAG_MOB_FIRSTFIND_STATE);
				CombineProto<SC_MONSTERSTATE>(GetCopyBuffer(), GetPacketHead(), scMonsterState, TAG_SC_MONSTERSTATE);
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				m_isInitStart = true;
			}
		}
	}

	void CSarcophagus::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);
	}

	bool CSarcophagus::IsHit(APawn* _pPawn, const _double& _dTimeDelta)
	{
		return false;
	}

	void CSarcophagus::Collision(APawn* _pPawn, const _double& _dTimeDelta)
	{
	}

	void CSarcophagus::LastBehavior()
	{
		if (true == IsPermanentDisable())
			return;

		ActivePermanentDisable();
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		SC_MONSTERSTATE scMonsterState;
		PROTOFUNC::MakeScMonsterState(&scMonsterState, GetSessionID(), 0, 0, TAG_MOB_DISABLE_STATE);
		CombineProto<SC_MONSTERSTATE>(GetCopyBuffer(), GetPacketHead(), scMonsterState, TAG_SC_MONSTERSTATE);
		spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
	}

	void CSarcophagus::Free()
	{
	}
}