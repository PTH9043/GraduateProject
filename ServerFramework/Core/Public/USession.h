#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H

#include "UObject.h"


BEGIN(Core)
class UService;

/*
@ Date: 2023-01-02, Writer: 박태현
@ Explain
- 클라이언트와 통신하기 위한 Session 클래스이고 가상함수로 Server 솔루션에서 해당 클래스를 부모로 자식 클래스를 
정의해주어야 한다.
*/
class CACHE_ALGIN_CORE_DLL USession abstract : public UObject  {
	using BUFFER = ARRAY<_char, MAX_BUFFER_LENGTH>;
	using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
	using TCPSOCKET = Asio::ip::tcp::socket;
public:
	USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET&& _TcpSocket, SESSIONTYPE _SessionType, SESSIONID _ID, SHPTR<UService> _spService);
	DESTRUCTOR(USession)
public: 
	virtual _bool Start() PURE;
	// 클라이언트에서 전송된 버퍼를 읽는 함수
	virtual void ReadData() PURE;
	// 클라이언트에게 버퍼를 조합하여 전송하는 함수
	virtual _bool WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	virtual void Disconnect() PURE;
public: /*Get Set */
	const SESSIONID GetID() const { return m_ID; }
	const SESSIONTYPE GetSessionType() const { return m_SessionType; }

protected:
	void PacketCombine(MOVE UBuffer&& _Buffer);
	virtual _bool ProcessPacket(_char* _pPacket, const PACKETHEAD& _PacketHead) PURE;
	void CombineSendBuffer(_char* _pPacket, const PACKETHEAD& _PacketHead);
	// 함수를 받아서 Tcp를 통해 IOContext에 함수 등록
	void RunReceiveFunc(const SENDRECVFUNC& _Func);
	// 함수를 받아서 Tcp를 통해 IOContext에 함수 등록
	void RunSendFunc(const SENDRECVFUNC& _Func);
	void Leave();

	template<class T>
	void CombineProto(REF_OUT UBuffer& _Buffer, REF_OUT PACKETHEAD& _PacketHead, T _data, short _tag)
	{
		_data.SerializePartialToArray((void*)&_Buffer.GetBuffer()[0], static_cast<_llong>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		_PacketHead = PACKETHEAD{ size, _tag };
	}

	TCPSOCKET& GetTcpSocket(REF_RETURN) { return m_TcpSocket; }
	SHPTR<UService> GetService() const { return m_wpService.lock(); }
	BUFFER& GetRecvBuff(REF_RETURN) { return m_RecvBuffer; }
	BUFFER& GetSendBuff(REF_RETURN) { return m_SendBuffer; }
private:
	virtual void Free() override;

private:
	SESSIONID						m_ID;
	SESSIONTYPE				m_SessionType;
	TCPSOCKET					m_TcpSocket;
	_llong								m_CurBuffuerLocation;
	// Buffer 모음
	TOTALBUFFER				m_TotalBuffer;
	BUFFER							m_RecvBuffer;
	BUFFER							m_SendBuffer;
	// Service
	WKPTR<UService>		m_wpService;
};

END

#endif //  _SERVERFRAMEWORK_CORE_PUBLIC_USESSION_H
