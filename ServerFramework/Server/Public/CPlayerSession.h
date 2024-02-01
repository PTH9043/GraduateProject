#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H

#include "ASession.h"

BEGIN(Core)
class AService;
END

BEGIN(Server)
/*
@ Date: 2023-01-02, Writer: 박태현
@ Explain
- Client에 존재하는 플레이어들과 통신하기 위한 클래스
*/
class  CPlayerSession final : public Core::ASession {
public:
	CPlayerSession(SESSION_CONSTRUCTOR);
	NO_COPY(CPlayerSession)
	DESTRUCTOR(CPlayerSession)
public:
	virtual _bool Start() override;
	// 클라이언트에서 전송된 버퍼를 읽는 함수
	virtual void ReadData() override;
	// 클라이언트에게 버퍼를 조합하여 전송하는 함수
	virtual _bool WriteData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
	virtual void Disconnect() override;
	virtual void ConnectTcpSocket() override;
protected:
	virtual _bool ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead) override;
private:
	virtual void Free() override;

private:
	_uint a{ 0 };
};


END
#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CPLAYERSESSION_H