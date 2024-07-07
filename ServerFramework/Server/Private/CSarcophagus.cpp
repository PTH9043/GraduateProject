#include "ServerDefines.h"
#include "CSarcophagus.h"
#include "CMummy.h"
#include "ATransform.h"
#include "AAnimator.h"
#include "CSacrophagusAnimController.h"
#include "ACoreInstance.h"

namespace Server
{
	CSarcophagus::CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType) :
		AMonster(OBJCON_CONDATA, _ID), m_eSarcophagusType{_eSarcophagusType}
	{
		if (SARCO_LAYING == m_eSarcophagusType)
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_STANDING);
		}
	}

	_bool CSarcophagus::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);
		ASSERT_CRASH(_ReceiveDatas.size() >= 0);
		SESSIONID* pSessionID = static_cast<SESSIONID*>(_ReceiveDatas[1]);

		SHPTR<CSacrophagusAnimController> spSacrophagusAnimController = nullptr;

		switch (m_eSarcophagusType)
		{
		case SARCO_LAYING:
		{
			spSacrophagusAnimController =	Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(), 
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusLaying_FBX.bin");
			m_spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_LAYING);
		}
		break;
		case SARCO_STANDING:
		{
			spSacrophagusAnimController = Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(),
				"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusStanding_FBX.bin");
			m_spMummy = Create<CMummy>(GetCoreInstance(), *pSessionID, MUMMYTYPE::MUMMY_STANDING);
		}
		break;
		}
		
		SetAnimController(spSacrophagusAnimController);
		m_spMummy->Start({ _ReceiveDatas[0] });	
		GetTransform()->SetNewWorldMtx(m_spMummy->GetTransform()->GetWorldMatrix());
		GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
		BringCellIndextoPosition();
		GetCoreInstance()->InsertMobObject(m_spMummy->GetSessionID(), m_spMummy);
		m_spMummy->GetTransform()->TranslateDir((m_spMummy->GetTransform()->GetLook()), 1, 12);
		return S_OK;
	}

	void CSarcophagus::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{

	}

	void CSarcophagus::Free()
	{
	}
}