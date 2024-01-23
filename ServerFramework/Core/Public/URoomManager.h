#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UROOMMANAGER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UROOMMANAGER_H

#include "UBase.h"

BEGIN(Core)
class URoom;

class URoomManager final : public UBase {
public:
	using ROOMCONTAINER = CONHASHMAP<_llong, SHPTR<URoom>>;
public:
	URoomManager();
	NO_COPY(URoomManager)
	DESTRUCTOR(URoomManager)
public:


private:
	virtual void Free() override;
private:
	ROOMCONTAINER		m_RoomContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UROOMMANAGER_H