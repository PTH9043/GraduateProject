#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H

#include "ACoreObject.h"

BEGIN(Core)
class ATransform;
class ACollider;
class ASpace;

class CORE_DLL AGameObject abstract : public ACoreObject {
public:
	AGameObject(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(AGameObject)
public:
	virtual _bool Start() PURE;
	void CreateColliderAndTransform(const COLLIDERINFO& _ColliderInfo, const Vector3& _vPos);
	void BringSpaceIndex(SHPTR<ASpace> _spSpace);
public:/*Get Set */
	const SESSIONID& GetSessionID() const { return m_SessionID; }
	const SESSIONTYPE& GetSessionType() const { return m_SessionType; }
	const _int& GetSpaceIndex() const { return m_SpaceIndex; }
	const CHARSTATUS& GetCharStatus() const { return m_CharStatus; }
	SHPTR<ATransform> GetTransform() const { return m_spTransform; }
	SHPTR<ACollider> GetCollider() const { return m_spCollider; }
	const _int GetGameObjectType() const { return m_GameObjectType; }
	const _int GetCellIndex() const { return m_CellIndex; }
	const _bool IsJumpable() const { return m_isJumpable; }
	const _float GetMoveSpeed() const { return m_fMoveSpeed; }
	const _float GetRunSpeed() const { return m_fRunSpeed; }
	const _int GetCurOnCellIndex() const { return m_iCurOnCellIndex; }

	void SetJumpable(const _bool _isJumpable) { this->m_isJumpable = _isJumpable; }
protected: /* Get Set */
	void SetSessionID(const SESSIONID& _SessionID) { this->m_SessionID = _SessionID; }
	void SetSessionType(const SESSIONTYPE& _SessionType) { this->m_SessionType = _SessionType; }
	void SetSpaceIndex(const _int& _SpaceIndex) { this->m_SpaceIndex = _SpaceIndex; }
	void SetCharStatus(const CHARSTATUS& _charStatus) { ::memcpy(&m_CharStatus, &_charStatus, sizeof(CHARSTATUS)); }
	void SetGameObjectType(const _int _GameObjectType) { this->m_GameObjectType = _GameObjectType; }
	void SetCellIndex(const _int _CellIndex) { this->m_CellIndex = _CellIndex; }
	void SetMoveSpeed(const _float _fMoveSpeed) { this->m_fMoveSpeed = _fMoveSpeed; }
	void SetRunSpeed(const _float _fRunSpeed) { this->m_fRunSpeed = _fRunSpeed; }
	void SetCurOnCellIndex(const _int _iCurOnCellIndex) { this->m_iCurOnCellIndex = _iCurOnCellIndex; }
private:
	virtual void Free() override;
private:
	SESSIONID							m_SessionID;
	SESSIONTYPE					m_SessionType;
	_int										m_SpaceIndex;
	_int										m_GameObjectType;
	_int										m_CellIndex;
	_bool									m_isJumpable;
	CHARSTATUS					m_CharStatus;

	SHPTR<ATransform>		m_spTransform;
	SHPTR<ACollider>			m_spCollider;
	// ÇöÀç ¼¿ À§¿¡ ÀÖ´Â ÀÎµ¦½º
	_int										m_iCurOnCellIndex;

	_float									m_fMoveSpeed;
	_float									m_fRunSpeed;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AGAMEOBJECT_H