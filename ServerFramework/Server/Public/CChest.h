#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CCHEST_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CCHEST_H

#include "CServerMonster.h"

BEGIN(Server)

class CChest final : public CServerMonster {
public:
	CChest(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CChest)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
protected:
	virtual void LastBehavior() override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CCHEST_H