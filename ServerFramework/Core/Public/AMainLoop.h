#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H

#include "ACoreObject.h"

BEGIN(Core)
class AService;
class AGameTimer;
class AGameObject;

class AMainLoop final : public ACoreObject {
	using COLLISIONLIST = CONSET<AGameObject*>;
public:
	AMainLoop(OBJCON_CONSTRUCTOR, Asio::io_context& _context, SHPTR<AService> _spServerService);
	DESTRUCTOR(AMainLoop)
public:
	void RegisterTimer(_int _iTime);
	void TimerThread(const boost::system::error_code& _error);
	void InsertPawnCollisionList(AGameObject* _pGameObject);
	void InsertStaticObjCollisionList(AGameObject* _pGameObject);
private:
	virtual void Free() override;
private:
	Asio::steady_timer 				m_SteadyTimer;

	SHPTR<AGameTimer>			m_spGameTimer;
	WKPTR< AService>				m_wpServerService;

	COLLISIONLIST							m_PawnCollisionList;
	COLLISIONLIST							m_StaticObjCollisionList;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AMAINLOOP_H