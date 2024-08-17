#include "ServerDefines.h"
#include "CCoreAnubis.h"
#include "ACollider.h"

namespace Server
{
	CCoreAnubis::CCoreAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		CServerStaticObject(OBJCON_CONDATA, _ID)
	{
		SetStaticObjType(TAG_COREANUBIS);
	}

	_bool CCoreAnubis::Start(const VOIDDATAS& _ReceiveDatas)
	{
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORINTERACTION_BARS, ACollider::TYPE_OBB,
			COLLIDERDESC{ {-10.f, 10.f, 15.f}, {12.5f, 12.5f, 12.5f} });

	//	GetActiveTimerRefP(REF_RETURN).SetStandardTime(5.f);
		return __super::Start(_ReceiveDatas);
	}

	void CCoreAnubis::State(SHPTR<ASession> _spSession)
	{
		__super::State(_spSession);
	}

	void CCoreAnubis::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{

	}

	void CCoreAnubis::Free()
	{
	}

}