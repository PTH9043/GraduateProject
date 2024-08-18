#include "ServerDefines.h"
#include "CCoreMinotaur.h"
#include "ACollider.h"

namespace Server {
	CCoreMinotaur::CCoreMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		CServerStaticObject(OBJCON_CONDATA, _ID)
	{
		SetStaticObjType(TAG_COREMINOTAUR);
	}

	_bool CCoreMinotaur::Start(const VOIDDATAS& _ReceiveDatas)
	{
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORINTERACTION_COREMINOTAUR, ACollider::TYPE_OBB,
			COLLIDERDESC{ {-10.f, 10.f, 15.f}, {12.5f, 12.5f, 12.5f} });

		GetActiveTimerRefP(REF_RETURN).SetStandardTime(5.f);
		SetActiveRange(20.f);
		return __super::Start(_ReceiveDatas);
	}

	void CCoreMinotaur::State(SHPTR<ASession> _spSession)
	{
		__super::State(_spSession);

		if (true == IsCurrentFindPlayer())
		{
			std::cout << "CCoreMinotaur" << "\n";;
		}
	}

	void CCoreMinotaur::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CCoreMinotaur::Free()
	{
	}

}