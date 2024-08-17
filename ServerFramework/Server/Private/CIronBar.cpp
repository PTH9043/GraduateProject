#include "ServerDefines.h"
#include "CIronBar.h"
#include "ACollider.h"

namespace Server {

	CIronBar::CIronBar(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		CServerStaticObject(OBJCON_CONDATA, _ID)
	{
		SetStaticObjType(TAG_IRONBAR);
	}

	_bool CIronBar::Start(const VOIDDATAS& _ReceiveDatas)
	{
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB, 
			COLLIDERDESC{ {0.f, 0.f, 0.f}, {410.f, 350.f, 0.f} });
		
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORINTERACTION_BARS, ACollider::TYPE_OBB,
			COLLIDERDESC{ {0.f, 0.f, 0.f}, {410.f, 350.f, 100.f} });

		return __super::Start(_ReceiveDatas);
	}

	void CIronBar::State(SHPTR<ASession> _spSession)
	{
		__super::State(_spSession);
	}

	void CIronBar::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CIronBar::LastBehavior()
	{
		std::cout << "Last" << "\n";
	}

	void CIronBar::Free()
	{
	}

}