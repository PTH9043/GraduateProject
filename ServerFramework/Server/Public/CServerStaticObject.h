#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERSTATICOBJECT_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVERSTATICOBJECT_H

#include "AStaticObject.h"

BEGIN(Server)

class CServerStaticObject abstract : public AStaticObject {
public:
	CServerStaticObject(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CServerStaticObject)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession) PURE;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
public: /* get set*/
	const _bool IsDoneInteractStaticObject() const { return m_isDoneInteractStaticObject; }
	const _string& GetRootName() const { return m_strRoomName; }
	const _int GetStaticObjectType() const { return m_StaticObjType; }
protected:
	virtual void LastBehavior() override;
protected: /* get set */
	void SetStaticObjType(const _int _StaticObjType) { m_StaticObjType = _StaticObjType; }
	void SetDoneInteractiveStaticObject(const _bool _isDoneInteractiveStaticObject) { this->m_isDoneInteractStaticObject = _isDoneInteractiveStaticObject; }

	CUSTIMER& GetActiveTimerRefP(REF_RETURN) { return m_ActiveTimer; }
private:
	virtual void Free() override;
private:
	_int								m_StaticObjType;
	ATOMIC<_bool>		m_isDoneInteractStaticObject;
	_string						m_strRoomName;
	CUSTIMER					m_ActiveTimer;
	
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVEROBJECT_H