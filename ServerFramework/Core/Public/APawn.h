#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H

#include "AGameObject.h"

BEGIN(Core)
class AAnimator;
class AAnimController;
class ANavigation;

class CORE_DLL APawn abstract : public AGameObject {
public:
	APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(APawn)
public:
	void Placement(_int _CellIndex);
	Vector3 BringCellIndextoPosition();
	virtual bool IsHit(APawn* _pPawn, const _double& _dTimeDelta) PURE;
	void GameObjectHeal(_float _fHeal);
	void Damaged(_float _fDamage);
public: /* get set */
	SHPTR<AAnimController> GetAnimController() { return m_spAnimController; }
	const CHARSTATUS& GetCharStatus() const { return m_CharStatus; }
	_bool IsHitState() const { return m_isHitState.load(); }

	virtual void SetActive(const _bool _isActive) override;
	void SetHitState(const _bool _isHit) { this->m_isHitState = _isHit; }
protected:
	// Damaged
	virtual void Collision(APawn* _pPawn, const _double& _dTimeDelta) PURE;
	void RestrictPositionToNavi();
protected: /* get set*/
	void SetCharStatus(const CHARSTATUS& _charStatus) { ::memcpy(&m_CharStatus, &_charStatus, sizeof(CHARSTATUS)); }
	void SetAnimController(SHPTR<AAnimController> _spAnimController) { this->m_spAnimController = _spAnimController; }
	SHPTR<ANavigation> GetNavigation() { return m_spNavigation; }
private:
	virtual void Free() override;
private:
	// Animator
	SHPTR< AAnimController>		m_spAnimController;
	SHPTR<ANavigation>					m_spNavigation;
	ATOMIC<_bool>							m_isHitState;
	CHARSTATUS								m_CharStatus;
	ATOMIC<_bool>							m_isDead;
};	


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H