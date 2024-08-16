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
	_bool ComputeCheckYPlayerPos(const _float _fMin, const _float _fMax);
public:
	const Vector3& GetOriginPos() const { return m_vOriginPos; }
	const Vector3& GetOriginToDirection() const { return m_vOriginToDir; }
	const _float GetOriginPosToDot() const { return m_fOriginPosToDot; }
	const _bool IsShield() const { return m_isShield; }
	const _bool IsFireAttack() const { return m_isFireAttack; }

	void SetShield(_bool _isShield) { this->m_isShield = _isShield; }
protected:
	virtual void TickUpdateBehavior(const _double& _dTimeDelta, SHPTR<ASession> _spSession) override;
private:
	virtual void Free() override;
private:
	Vector3				m_vOriginPos;
	Vector3				m_vOriginToDir;
	_float					m_fOriginPosToDot;

	_bool					m_isShield;
	_bool					m_isFireAttack;
};

END

#endif