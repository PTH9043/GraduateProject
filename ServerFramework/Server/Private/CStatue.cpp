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
		SetActiveRange(12);

		return __super::Start(_ReceiveDatas);
	}

	void CStatue::State(SHPTR<ASession> _spSession)
	{
		__super::State(_spSession);
	}

	void CStatue::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CStatue::Free()
	{
	}

}