#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSTATUE_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSTATUE_H

#include "CServerStaticObject.h"

BEGIN(Server)

class CStatue final : public CServerStaticObject {
public:
	CStatue(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CStatue)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSTATUE_H