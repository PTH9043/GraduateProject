#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UOBBCOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UOBBCOLLIDER_H

#include "UCollider.h"

BEGIN(Core)

/*
@ Date: 2023-01-19, Writer: 박태현
@ Explain
- 객체의 OBB충돌을 하기 위한 클래스이다.
*/
class  CORE_DLL  UOBBCollider final : public UCollider {
public:
	enum { PROJ_AXIS_CNT = 15};
public:
	UOBBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(UOBBCollider)
	DESTRUCTOR(UOBBCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<UCollider> _spCollider) override;
protected:
	// OBB를 축에 투영하여 구간을 계산하는 함수이다. 
	_float ProjectionOBBToAxis(const Vector3& _vPos, const Vector3& _vScale,
		const Vector3& _vAxis);
private:
	virtual void Free() override;

};

END

#endif  // _SERVERFRAMEWORK_CORE_PUBLIC_UOBBCOLLIDER_H