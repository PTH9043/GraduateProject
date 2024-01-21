#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H

#include "UService.h"

BEGIN(Core)
class UNavigation;
/*
@ Date: 2024-01-05, Writer: 박태현
@ Explain
-  ServerService로 재정의해서 사용한다.  Server Framework에서 재정의 해서 사용해야한다. 
*/
class CORE_DLL UServerService abstract  : public UService {
public: /* USING */
	using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<USession>>;
public: 
	UServerService(OBJCON_CONSTRUCTOR, const _string& _strNavigationPath = "");
	DESTRUCTOR(UServerService)
public:
	virtual _bool NativeConstruct() PURE;
	virtual _bool Start() override;
public:
	/* AWS */
	virtual SHPTR<USession> FindSession(const SESSIONID _SessionID) override;
	virtual void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead) override;
	// 여기서는 Session 자체를 제거
	virtual void LeaveService(const SESSIONID _SessionID) override;
	virtual void InsertSession(SESSIONID _SessionID, SHPTR<USession> _spSession) override;
protected:
	// Thread에 집어넣을 함수
	static void ThreadFunc(void* _spService);
protected: /* get set */
	TCPACCEPTOR& GetTcpAccepctor(REF_RETURN) { return m_TcpAcceptor; }
private:
	virtual void Free() PURE;
private:
	// Tcps
	TCPACCEPTOR							m_TcpAcceptor;
	// Session Conatiner
	SESSIONCONTAINER				m_SessionContainer;
	// Navigation
	SHPTR<UNavigation>				m_spNavigation;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_USERVERSERVICE_H