#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H

#include "ACoreBase.h"

BEGIN(Core)

/*
@ Date: 2023-01-18, Writer: 박태현
@ Explain
- 객체의 충돌처리를 하기 위한 클래스이다. 
*/
class CORE_DLL ACollider abstract : public ACoreBase {
public:
	enum { AXIS_CNT = 3 };
public:
	ACollider(COLLIDERTYPE _ColliderType,  const Vector3 _vOriPos, const Vector3 _vOriScale);
	DESTRUCTOR(ACollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) PURE;
	virtual _bool IsCollision(SHPTR<ACollider> _spCollider) PURE;
public: /* get set*/
	const Vector3 GetOriginPosition() const { return m_vOriginPosition; }
	const Vector3 GetOriginScale() const { return m_vScale; }
	_matrix  GetChangeMatrix() const { return m_mChangeMatrix;  }
	const Vector3 GetChangeScale() const { return m_vChangeScale; }

	const COLLIDERTYPE GetColliderType() const { return m_ColliderType; }

protected:
	void UpdateChangeValue(const _matrix& _ChangeMatrix, const Vector3& _vChangeScale);
	void UpdateChangeValue(const Vector3& _vChangePos, const Vector3& _vChangeScale);
private:
	virtual void Free() PURE;
private:
	COLLIDERTYPE		m_ColliderType;
	// Origin 
	Vector3					m_vOriginPosition;
	Vector3					m_vScale;
	// Change 
	_matrix					m_mChangeMatrix;
	Vector3					m_vChangeScale;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOLLIDER_H