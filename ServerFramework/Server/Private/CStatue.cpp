#include "ServerDefines.h"
#include "CStatue.h"
#include "ACollider.h"
#include "ASession.h"
#include "ACoreInstance.h"

namespace Server {

	CStatue::CStatue(OBJCON_CONSTRUCTOR, SESSIONID _ID) : 
		CServerStaticObject(OBJCON_CONDATA, _ID)
	{
		SetStaticObjType(TAG_STATUE);
	}

	void CStatue::Tick(const _double& _dTimeDelta)
	{
		__super::Tick(_dTimeDelta);
	}

	_bool CStatue::Start(const VOIDDATAS& _ReceiveDatas)
	{
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORINTERACTION_STATUE, ACollider::TYPE_OBB,
			COLLIDERDESC{ {0.f, 0.f, 0.f}, {15.f, 30.f, 15.f} });

		return __super::Start(_ReceiveDatas);
	}

	void CStatue::State(SHPTR<ASession> _spSession)
	{
		FindPlayer(_spSession);
		RETURN_CHECK(false == IsCurrentFindPlayer(), ;);
		_int enable = 0;

		SESSIONID sessionID = _spSession->GetSessionID();
		_int KeyState = _spSession->GetKeyState();
		if (KeyState == KEYBOARD_F)
		{
			SetDoneInteractiveStaticObject(true);
		}
		else
		{
			SetDoneInteractiveStaticObject(false);
			GetActiveTimerRefP().ResetTimer();
		}
		_bool IsPass = GetActiveTimerRefP(REF_RETURN).IsOver();
		enable = (true == IsPass ? 2 : (true == IsDoneInteractStaticObject() ? 1 : 0));

		SC_STATICOBJFIND scStaticObjectFind;
		PROTOFUNC::MakeScStaticObjFind(&scStaticObjectFind, GetSessionID(), enable);
		CombineProto<SC_STATICOBJFIND>(GetCopyBuffer(), GetPacketHead(), scStaticObjectFind,
			TAG_SC_STATICOBJFIND);
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
		scStaticObjectFind.Clear();
	}

	void CStatue::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CStatue::Free()
	{
	}

}