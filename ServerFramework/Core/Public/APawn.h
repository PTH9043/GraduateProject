#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H

#include "AGameObject.h"

BEGIN(Core)
class AAnimator;
class AAnimController;
class ANavigation;
class ACell;

class CORE_DLL APawn abstract : public AGameObject {
public:
	APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(APawn)
public:
	void Placement(_int _CellIndex);
	Vector3 BringCellIndextoPosition();
	virtual bool IsHit(APawn* _pPawn, const _double& _dTimeDelta) PURE;

	void HealHp(const _float _fHeal);
	void DamageToEnemy(const _float _fDamge);
public: /* get set */
	SHPTR<AAnimController> GetAnimController() { return m_spAnimController; }
	const CHARSTATUS& GetCharStatus() const { return m_CharStatus; }
	const _bool IsDamaged() const { return m_isDamaged; }
	const _bool IsDead() const { return m_isDead; }
	const _bool IsDeadStateEnable() const { return m_isDeadStateEnable; }
	SHPTR<ACell> GetCurCell();

	virtual void SetActive(const _bool _isActive) override;
	void SetDamaged(const _bool _isDamaged) { this->m_isDamaged = _isDamaged; }
	void DeadStateEnable() { this->m_isDeadStateEnable = true; }
protected:
	// Damaged
	virtual void Collision(APawn* _pPawn, const _double& _dTimeDelta) PURE;
	void RestrictPositionToNavi();
protected: /* get set*/
	void SetAnimController(SHPTR<AAnimController> _spAnimController) { this->m_spAnimController = _spAnimController; }
	SHPTR<ANavigation> GetNavigation() { return m_spNavigation; }

	void SetPrevPosition(const Vector3 _vPrevPos) { this->m_vPrevPosition = _vPrevPos; }
	const Vector3 GetPrevPosition() const { return m_vPrevPosition; }

	void SetCharStatus(const CHARSTATUS& _charStatus) { ::memcpy(&m_CharStatus, &_charStatus, sizeof(CHARSTATUS)); }
private:
	virtual void Free() override;
private:
	Vector3											m_vPrevPosition;
	// Animator
	SHPTR< AAnimController>		m_spAnimController;
	SHPTR<ANavigation>				m_spNavigation;
	CHARSTATUS								m_CharStatus;
	ATOMIC<_bool>							m_isDamaged;
	ATOMIC<_bool>							m_isDead;
	// Dead state È°¼ºÈ­
	ATOMIC<_bool>							m_isDeadStateEnable;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H