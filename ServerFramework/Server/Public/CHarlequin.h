#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUIN_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUIN_H

#include "CServerMonster.h"

BEGIN(Server)
class CShurikenThrowing;

class CHarlequin final : public CServerMonster {
	static constexpr int THROWING_NUM{ 6 };
	using SHURIKENCONTAINER = ARRAY<SHPTR<CShurikenThrowing>, THROWING_NUM>;
public:
	CHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CHarlequin)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
public:
	void SetDontMove(const _bool _isDontMove) { this->m_isDontMove = _isDontMove; }
	_bool IsDontMove() const { return m_isDontMove; }
public: 
	SHPTR<CShurikenThrowing> GetShurikenThrowing(_int _iIndex);
protected:
	virtual void TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession) override;
	virtual void MoveAlongPath(const _double& _dTimeDelta) override;
	virtual void LastBehavior() override;
private:
	virtual void Free() override;
private:
	SHURIKENCONTAINER		m_ShurikenContainer;
	_bool										m_isDontMove;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CHARLEQUIN_H