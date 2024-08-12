#include "ServerDefines.h"
#include "CCoreHarlequin.h"
#include "ACollider.h"

namespace Server
{
	CCoreHarlequin::CCoreHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		CServerStaticObject(OBJCON_CONDATA, _ID)
	{
		SetStaticObjType(TAG_COREHARLEQUIN);
	}

	_bool CCoreHarlequin::Start(const VOIDDATAS& _ReceiveDatas)
	{
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORINTERACTION_COREHARLEQUIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {-10.f, 10.f, 15.f}, {12.5f, 12.5f, 12.5f} });

		return __super::Start(_ReceiveDatas);
	}

	void CCoreHarlequin::State(SHPTR<ASession> _spSession)
	{
	}

	void CCoreHarlequin::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CCoreHarlequin::Free()
	{
	}

}