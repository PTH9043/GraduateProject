#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREANUBIS_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREANUBIS_H

#include "CServerStaticObject.h"

BEGIN(Server)

class CCoreAnubis final : public CServerStaticObject {
public:
	CCoreAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CCoreAnubis)
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