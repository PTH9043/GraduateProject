#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H

#include "UBase.h"

BEGIN(Core)
class UTransform;
class UCollider;
class USession;
/*
@ Date: 2023-01-21, Writer: 박태현
@ Explain
- 게임상의 공간을 정의하여 시야 확보를 하기 위한 클래스이다.
전체 오브젝트를 검색하는 것은 낭비다. 그래서 논리적인 공간을 나누어서 검색을 하는 것이 합리적이라고 생각해서
해당 클래스를 정의했다.
*/
class CORE_DLL  USpace final : public UBase {
public:
	using SESSIONCONTAINER = CONLIST<SHPTR<USession>>;
public:
	using SPACECHILD = ARRAY<WKPTR<USpace>, MAX_OCTREENODE_LENGTH>;
public:
	USpace();
	NO_COPY(USpace)
	DESTRUCTOR(USpace)
public:
	// Insert
	void InsertSession(SHPTR<UCollider>	 _spCollider, SHPTR<USession> _spSession);
	void Rebalance(SHPTR<UCollider>	 _spCollider, SHPTR<USession> _spSession);
	
private:
	virtual void Free() override;
private:
	_llong											m_SpaceIndex;
	// Child + Parents
	WKPTR<USpace>					m_wpParents;
	SPACECHILD								m_SpaceChild;
	// Components
	SHPTR<UTransform>				m_spTransform;
	SHPTR<UCollider>					m_spCollider;
	// LIST
	SESSIONCONTAINER				m_SessionContainer;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACE_H