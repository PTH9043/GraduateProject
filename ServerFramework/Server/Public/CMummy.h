#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H

#include "CServerMonster.h"

BEGIN(Server)

class CMummy final : public CServerMonster {
public:
	CMummy(OBJCON_CONSTRUCTOR, SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CMummy)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void Tick(const _double& _dTimeDelta) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
	virtual void ProcessPacket(_int _type, void* _pData) override;
	virtual void Collision(AGameObject* _pGameObject, const _double& _dTimeDelta) override;
public: /* get set*/
	MUMMYTYPE GetMummyType() const { return m_eMummyType; }
private:
	virtual void Free() override;
private:
	MUMMYTYPE					m_eMummyType;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMY_H