#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSHURIKENTHROWING_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSHURIKENTHROWING_H

#include "AStaticObject.h"

BEGIN(Server)

class CShurikenThrowing final : public AStaticObject {
public:
	CShurikenThrowing(OBJCON_CONSTRUCTOR, SESSIONID _ID);
	DESTRUCTOR(CShurikenThrowing)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	void ThrowShurikens(const _double& _dTimeDelta, const Vector3& _vDir);
	virtual void State(SHPTR<ASession> _spSession) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
public: /* get set */
	_bool IsThrown() const { return m_isThrown; }
	void SetThrown(const _bool _isThrown) { this->m_isThrown = _isThrown; }

	const _float GetTraveledDistance() const { return m_fTraveledDistance; }
	void ResetTraveledDistance() { m_fTraveledDistance = 0.0f; }
protected:
	virtual void LastBehavior() override;
private:
	virtual void Free() override;
private:
	_float			m_fTraveledDistance;
	_bool			m_isThrown;
	Vector3		m_vMinVertexPos;
	Vector3		m_vMaxVertexPos;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSERVEROBJECT_H