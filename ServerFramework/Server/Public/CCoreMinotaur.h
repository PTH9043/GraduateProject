#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREMINOTAUR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREMINOTAUR_H

#include "CServerStaticObject.h"

BEGIN(Server)

class CCoreMinotaur final : public CServerStaticObject {
public:
	CCoreMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CCoreMinotaur)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:

};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREMINOTAUR_H