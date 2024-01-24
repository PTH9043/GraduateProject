#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USPACEMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USPACEMANAGER_H

#include "UBase.h"

BEGIN(Core)
class USpace;
class USession;

/*
@ Date: 2023-01-22, Writer: 박태현
@ Explain
- Space를 관리하는 매니저 클래스
*/
class USpaceManager final : public UBase {
public:
	using SPACECONTAINER = CONVECTOR<SHPTR<USpace>>;
public:
	USpaceManager();
	NO_COPY(USpaceManager)
	DESTRUCTOR(USpaceManager)
public:
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);
	void InsertSessionInSpace(SHPTR<USession >_spSession);

protected:
	void BuildTreeRecursively(const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents = nullptr);

private:
	virtual void Free() override;
private:
	// Root 
	SHPTR<USpace>		m_spRootSpace;	
	SPACECONTAINER	m_SpaceContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USPACEMANAGER_H