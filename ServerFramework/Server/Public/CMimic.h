#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMIMIC_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMIMIC_H

#include "CServerMonster.h"

BEGIN(Server)

class CMimic final : public CServerMonster {
public:
	CMimic(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CMimic)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;

	void EnableOpen() { this->m_isOpeningState = true; }
public: /* get set */
	const _bool IsPressKeyEnable() const { return m_isPressKeyEnable; }
	const _bool IsOpeningState() const { return m_isOpeningState; }
protected:
	virtual void TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession) override;
private:
	virtual void Free() override;
private:
	static constexpr _float NUCKBACK_SPEED{ 20 };
	ATOMIC<_bool>				m_isPressKeyEnable;
	_bool									m_isOpeningState;
};


END

#endif 