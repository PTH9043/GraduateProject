#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ASTATICOBJECT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ASTATICOBJECT_H

#include "AGameObject.h"

BEGIN(Core)
class ASession;
class ACell;

class CORE_DLL AStaticObject abstract : public AGameObject {
public:
	AStaticObject(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(AStaticObject)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) PURE;
	void FindPlayer(SHPTR<ASession> _spSession);
	virtual void State(SHPTR<ASession> _spSession) PURE;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) PURE;
public: /* get set */
	const _bool IsCurrentFindPlayer() const { return m_isCurrentFindPlayer; }
protected: /* get set */
	void SetActiveRange(const _float _fActiveRange) { this->m_fActiveRange = _fActiveRange; }
private:
	virtual void Free() override;
private:
	_float								m_fActiveRange; 
	std::atomic_bool			m_isCurrentFindPlayer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ASTATICOBJECT_H