#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H

#include "ACollider.h"

BEGIN(Core)
/*
@ Date: 2023-01-19, Writer: 박태현
@ Explain
- 객체의 SphereCollider충돌을 하기 위한 클래스이다.
*/
class CORE_DLL ASphereCollider final : public ACollider {
public:
	ASphereCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(ASphereCollider)
	DESTRUCTOR(ASphereCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<ACollider> _spCollider) override;
private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPHERECOLLIDER_H