#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UTRANFORM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UTRANFORM_H

#include "UBase.h"

BEGIN(Core)

/*
@ Date: 2023-01-18, Writer: 박태현
@ Explain
- 객체의 위치를 정의하기 위한 클래스이다. 
*/
class CACHE_ALGIN_CORE_DLL UTransform final : public UBase {
public:
	UTransform();
	NO_COPY(UTransform)
	DESTRUCTOR(UTransform)
public:
	const Vector3 GetScale() const { return m_vOriginScale; }
	const Vector3 GetAngles() const;
	const _float	GetXAngle() const;
	const _float	GetYAngle() const;
	const _float	GetZAngle() const;

	const _matrix GetWorldMatrix() const { return m_mWorldMatrix; }
	const _matrix GetWorldMatrixTP() const { return m_mWorldMatrix.Transpose(); }
	const _matrix GetWorldMatrixInverse() const { return m_mWorldMatrix.Invert(); }

	const Vector3 GetRight() const {return m_mWorldMatrix.GetRight();}
	const Vector3 GetUp() const { return m_mWorldMatrix.GetUp(); }
	const Vector3 GetLook() const { return m_mWorldMatrix.GetLook(); }
	const Vector3 GetLeft() const { return m_mWorldMatrix.GetLeft(); }
	const Vector3 GetDown() const { return m_mWorldMatrix.GetDown(); }
	const Vector3 GetBack() const { return m_mWorldMatrix.GetBack(); }
	const Vector3 GetPos() const { return m_mWorldMatrix.GetPos(); }

	void SetRight(const Vector3 _vec) { m_mWorldMatrix.SetRight(_vec); }
	void SetUp(const Vector3 _vec) { m_mWorldMatrix.SetUp(_vec); }
	void SetLook(const Vector3 _vec) { m_mWorldMatrix.SetLook(_vec); }
	void SetPos(const Vector3 _vec) { m_mWorldMatrix.SetPos(_vec); }

	void RotateFix(const Vector3 _vStandardAngle, const _float _TurnAnge);
	// Just Angle
	void RotateFix(const Vector3 _vAngle);
	void RotateTurn(const Vector3 _vAxis, const _float _Angle);
	void RotateTurn(const Vector3 _vAngle);
	void LookAt(const Vector3 _vTargetPos);
	// Compute Distance
	const _float ComputeDistance(const Vector3 _vPos);
	const _float ComputeDistanceSq(const Vector3 _vPos);
private:
	virtual void Free() override;
private:
	_matrix				m_mWorldMatrix;
	Vector3				m_vOriginScale;
	_quaternion		m_Rotation;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UTRANFORM_H