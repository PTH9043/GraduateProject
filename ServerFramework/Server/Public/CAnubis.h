#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CANUBIS_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CANUBIS_H

#include "CServerMonster.h"

BEGIN(Server)

class CAnubis final : public CServerMonster {
public:
	CAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CAnubis)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
};

END

#endif