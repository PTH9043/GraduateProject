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
		MOBDATA* pMoveData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		SESSIONID* pSessionID = static_cast<SESSIONID*>(_ReceiveDatas[1]);

		SHPTR<CSacrophagusAnimController> spSacrophagusAnimController = nullptr;

		switch (m_eSarcophagusType)
		{
		case SARCO_LAYING:
		{
			spSacrophagusAnimController =	Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(), 
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusLaying_FBX.bin");
			m_spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_LAYING, GetMonsterJobTimer());
		}
		break;
		case SARCO_STANDING:
		{
			spSacrophagusAnimController = Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(),
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusStanding_FBX.bin");
			m_spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_STANDING, GetMonsterJobTimer());
		}
		break;
		}
		
		SetAnimController(spSacrophagusAnimController);
		m_spMummy->Start({ _ReceiveDatas[0] });	
		GetTransform()->SetNewWorldMtx(pMoveData->mWorldMatrix);
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		GetCoreInstance()->InsertMobObject(m_spMummy->GetSessionID(), m_spMummy);

		if (SARCO_LAYING == m_eSarcophagusType)
			m_spMummy->GetTransform()->TranslateDir((m_spMummy->GetTransform()->GetLook()), 1, 12);
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
				SHPTR<CMummy> spMummy = GetMummy();

				SC_MONSTERSTATE scMonsterState;
				PROTOFUNC::MakeScMonsterState(&scMonsterState, GetSessionID(), dDuration, AnimIndex, TAG_MOB_FIRSTFIND_STATE);
				CombineProto<SC_MONSTERSTATE>(GetCopyBuffer(), GetPacketHead(), scMonsterState, TAG_SC_MONSTERSTATE);
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				m_isInitStart = true;

				spMummy->SetFoundPlayerFirstTime(true);
			}
		}
	}

	void CSarcophagus::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);
	}

	void CSarcophagus::LastBehavior()
	{
		if (true == IsPermanentDisable())
			return;

		SHPTR<CMummy> spMummy = m_spMummy;
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