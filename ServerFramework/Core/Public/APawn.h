#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H

#include "AGameObject.h"

BEGIN(Core)
class AAnimator;
class AAnimController;
class ANavigation;
class ACell;
class ATransform;

class CORE_DLL APawn abstract : public AGameObject {
public:
	APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(APawn)
public:
	void Placement(_int _CellIndex);
	Vector3 BringCellIndextoPosition();

	_float HealHp(const _float _fHeal);
	_float DamageToEnemy(SHPTR<APawn> _spPawn);
	_float Damaged(float _fDamaged);

	_float OtherCharacterToDistance(SHPTR<ATransform> _spOtherTransform);
	_float OtherCharacterDirToLook(SHPTR<ATransform> _spOtherTransform);
	_float OhterCharacterDirToRight(SHPTR<ATransform> _spOtherTransform);
	_float OtherCharacterDirToLookConverter(SHPTR<ATransform> _spOtherTransform);
	Vector3 OtherCharacterDirToLookVectorF3(SHPTR<ATransform> _spOtherTransform);
	_float OtherCharacterToDistance(Vector3 _targetPos);
	_float OtherCharacterDirToLook(Vector3 _targetPos);
	_float OhterCharacterDirToRight(Vector3 _targetPos);
	_float OtherCharacterDirToLookConverter(Vector3 _targetPos);
	Vector3 OtherCharacterDirToLookVectorF3(Vector3 _targetPos);
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) PURE;
	void DeadStateEnable(const _double& _dTimeDelta);
	void ResetDamagedToEnemyTimer() { m_DamageToEnemyTimer.ResetTimer(); }
public: /* get set */
	SHPTR<AAnimController> GetAnimController() { return m_spAnimController; }
	const CHARSTATUS& GetCharStatus() const { return m_CharStatus; }
	const _bool IsDamaged() const { return m_isDamaged; }
	const _bool IsDead() const { return m_isDead; }
	const _bool IsDeadStateEnable() const { return m_isDeadStateEnable; }
	const _bool IsEnemyHitState() const { return m_isEnemyHitState; }
	SHPTR<ACell> GetCurCell();
	CUSTIMER& GetDamageToEnemyTimerRefP(REF_RETURN) { return m_DamageToEnemyTimer; }

	virtual void SetActive(const _bool _isActive) override;
	void SetDamaged(const _bool _isDamaged) { this->m_isDamaged = _isDamaged; }
	void SetEnemyHitState(const _bool _isEnemyHitState) { this->m_isEnemyHitState = _isEnemyHitState; }
	_bool IsDamagedToEnemyTimerPass() { return m_DamageToEnemyTimer.IsOver(); }
	void DisableDeadState() { m_isDead = false; m_isDeadStateEnable = false; m_DeadTimer.ResetTimer(); }
protected:
	void RestrictPositionToNavi();
	void RunningDamagedToEnemyTimer(const _double& _dTimeDelta);
protected: /* get set*/
	void SetAnimController(SHPTR<AAnimController> _spAnimController) { this->m_spAnimController = _spAnimController; }
	SHPTR<ANavigation> GetNavigation() { return m_spNavigation; }

	void SetPrevPosition(const Vector3 _vPrevPos) { this->m_vPrevPosition = _vPrevPos; }
	const Vector3 GetPrevPosition() const { return m_vPrevPosition; }

	void SetCharStatus(const CHARSTATUS& _charStatus) { ::memcpy(&m_CharStatus, &_charStatus, sizeof(CHARSTATUS)); }

	void SetHitAlreadyState(const _bool _isHitAreadyState) { this->m_isHitAlreadyState = _isHitAreadyState; }
	_bool IsHitAlreadyState() const { return m_isHitAlreadyState; }

	void SetCollisionState(const _bool _isCollisionState) { this->m_isCollisionState = _isCollisionState; }
	const _bool IsCollisionState() const { return m_isCollisionState; }
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
	ATOMIC<_bool>							m_isEnemyHitState;
	ATOMIC<_bool>							m_isHitAlreadyState;
	ATOMIC<_bool>							m_isCollisionState;

	CUSTIMER										m_DeadTimer;
	CUSTIMER										m_DamageToEnemyTimer;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H