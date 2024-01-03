#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H

#include "UService.h"

BEGIN(Core)

/*
@ Date: 2024-01-02, Writer: 박태현
@ Explain
-  ServerService로 재정의해서 사용한다.  Server Framework에서 재정의 해서 사용해야한다. 
*/
class CACHE_ALGIN_CORE_DLL UServerService abstract  : public UService {
public: /* USING */
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<USession>>;
public: 
	UServerService(OBJCON_CONSTRUCTOR);
	DESTRUCTOR(UServerService)
public:
	virtual _bool NativeConstruct() override;
	virtual _bool Start() override;
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void RemoveService(const SESSIONID _SessionID) override;
protected:
	virtual void InsertSession(SHPTR<USession> _spSession) override;
	virtual void AsyncAccept() PURE;
protected:
	// Thread에 집어넣을 함수
	static void ThreadFunc(void* _spServerService);
private:
	virtual void Free() PURE;
private:
	ATOMIC<_bool>				m_isConinueThread;
	// Session Conatiner
	SESSIONCONTAINER		m_SessionContainer;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H