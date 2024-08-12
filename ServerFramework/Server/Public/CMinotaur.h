#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAUR_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAUR_H

#include "CServerMonster.h"

BEGIN(Server)
class CMinotaurAnimController;

class CMinotaur final : public CServerMonster {
public:
	CMinotaur(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CMinotaur)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
protected:
	virtual void TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession) override;
private:
	virtual void Free() override;
private:
	static constexpr _float RUNNING_SPEED{ 30 };
	static constexpr _float WALKING_SPEED{ 5 };

	WKPTR< CMinotaurAnimController>			m_wpMinotaurAnimController;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMINOTAUR_H