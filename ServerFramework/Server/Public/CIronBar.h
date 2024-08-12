#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CIRONBAR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CIRONBAR_H

#include "CServerStaticObject.h"

BEGIN(Server)

class CIronBar final : public CServerStaticObject {
public:
	CIronBar(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CIronBar)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CIRONBAR_H