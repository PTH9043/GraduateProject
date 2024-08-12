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
#include "ASession.h"

namespace Server
{
	CSarcophagus::CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType, 
		SHPTR<AJobTimer> _spMonsterJobTimer) :
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer), m_eSarcophagusType{_eSarcophagusType}, m_isInitStart{false}
	{
		if (SARCO_LAYING == m_eSarcophagusType)
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_SARCOPHAGUS_STANDING);
		}

		UpdateFindRange(50.f, 90.f);
	}

	_bool CSarcophagus::Start(const VOIDDATAS& _ReceiveDatas)
	{
		ASSERT_CRASH(_ReceiveDatas.size() >= 0);
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
#ifndef CREATED_SERVERMOBDATA
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
		SetOwnerMonsterSessionID(spMummy->GetSessionID());
		GetCoreInstance()->InsertMobObject(spMummy->GetSessionID(), spMummy);

		if (SARCO_LAYING == m_eSarcophagusType)
			spMummy->GetTransform()->TranslateDir((spMummy->GetTransform()->GetLook() * -1), 1, 10);
#else
		SHPTR<CSacrophagusAnimController> spSacrophagusAnimController = Create<CSacrophagusAnimController>(GetCoreInstance(), ThisShared<CSarcophagus>(),
			"..\\..\\Resource\\Anim\\Sarcophagus\\", "SarcophagusLaying_FBX.bin", Matrix);
		SetAnimController(spSacrophagusAnimController);
#endif
		return __super::Start(_ReceiveDatas);
	}

	void CSarcophagus::RunningPermanentDisableSituation()
	{
	}

	void CSarcophagus::Tick(const _double& _dTimeDelta){ }

	void CSarcophagus::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		if (true == IsCurrentFindPlayer())
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

			SC_MONSTERFIND scMonsterFind;
			PROTOFUNC::MakeScMonsterFind(&scMonsterFind, GetSessionID(), _spSession->GetSessionID());
			CombineProto<SC_MONSTERFIND>(GetCopyBuffer(), GetPacketHead(), scMonsterFind, TAG_SC_MONSTERFIND);
			spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			ActivePermanentDisable();
		}
	}

	void CSarcophagus::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CSarcophagus::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CSarcophagus::LastBehavior()
	{
	}

	void CSarcophagus::Free()
	{
	}
}