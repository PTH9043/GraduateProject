#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H

#include "AMonster.h"

BEGIN(Server)

class CMummy final : public AMonster {
public:
	CMummy(OBJCON_CONSTRUCTOR, SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
private:
	virtual void Free() override;
private:
	MUMMYTYPE		m_eMumyType;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H