#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UCLIENTSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UCLIENTSERVICE_H

#include "UService.h"

BEGIN(Core)

/*
@ Date: 2024-01-02, Writer: 박태현
@ Explain
-  ServerService로 재정의해서 사용한다.  Server Framework에서 재정의 해서 사용해야한다.
*/
class CACHE_ALGIN_CORE_DLL UClientService abstract : public UService{
public:
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<USession>>;
public:
	UClientService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UClientService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() PURE;
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	virtual void LeaveService(const SESSIONID _SessionID) override;
protected:
	virtual void InsertSession(SHPTR<USession> _spSession) override;
	virtual void AsyncAccept() PURE;
	virtual SESSIONID GiveID() override;
private:
	virtual void Free() PURE;
private:
	SESSIONCONTAINER				m_SessionContainer;
	CONSTACK<SESSIONID>		m_RemainIDStack;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UCLIENTSERVICE_H 