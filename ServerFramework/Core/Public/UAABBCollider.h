#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAABBCOLLIDER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAABBCOLLIDER_H

#include "UCollider.h"

BEGIN(Core)
/*
@ Date: 2023-01-18, Writer: 박태현
@ Explain
- 객체의 AABB충돌을 하기 위한 클래스이다.
*/
class CACHE_ALGIN_CORE_DLL UAABBCollider final : public UCollider {
public:
	UAABBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale);
	NO_COPY(UAABBCollider)
	DESTRUCTOR(UAABBCollider)
public:
	virtual void Tick(const _matrix	_WorldMatrix) override;
	virtual _bool IsCollision(SHPTR<UCollider> _spCollider) override;

private:
	virtual void Free() override;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAABBCOLLIDER_H