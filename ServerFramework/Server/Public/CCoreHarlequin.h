#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREHARLEQUIN_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CCOREHARLEQUIN_H

#include "CServerStaticObject.h"

BEGIN(Server)

class CCoreHarlequin final : public CServerStaticObject {
public:
	CCoreHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CCoreHarlequin)
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