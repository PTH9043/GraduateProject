#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H

#include "UBase.h"

BEGIN(Core)
class UCoreInstance;

/*
@ Date: 2024-01-01
@ Writer: 박태현
@ Explain
- Socket Util들을 관리하는 구조체
해당 구조체는 프로젝트당 무조건 한 번만 만들어져야 한다.
*/
struct CACHE_ALGIN SOCKETUTILS {

	DECLARE_ONCEDATA

	SOCKETUTILS() :
		TcpAcceptor{ IoContext,Asio::ip::tcp::endpoint(Asio::ip::make_address(IP_ADDRESS),
			TCP_PORT_NUM) },
		TcpSocket{ IoContext },
		UdpSocket{ IoContext, Asio::ip::udp::endpoint(Asio::ip::make_address(IP_ADDRESS),
			UDP_PORT_NUM) }
	{
		CREATEINSTANCE_ONCEDATA;
	}

	// Boost/Asio Context
	IOContext							IoContext;
	// Tcps
	Asio::ip::tcp::acceptor	TcpAcceptor;
	Asio::ip::tcp::socket		TcpSocket;
	// UdpSocket
	Asio::ip::udp::socket		UdpSocket;

};
IMPLEMENT_ONCEDATA(SOCKETUTILS)



/*
@ Date: 2024-01-01
@ Writer: 박태현
@ Explain
- 어떻게 통신할지 정의한 AsioCore
*/
class CACHE_ALGIN_CORE_DLL UAsioCore final : public UBase {
	DECLARE_ONCEDATA
public:
	UAsioCore();
	NO_COPY(UAsioCore)
	DESTRUCTOR(UAsioCore)
		
public:
	_bool NativeConstruct();

private:
	virtual void Free() override;
private:
	SHPTR<UCoreInstance>		m_spCoreInstance;
	SOCKETUTILS						m_SocketUtils;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UASIOCORE_H