#pragma once

#include "ServerUtility.h"

BEGIN(Engine)

using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;

/*
@ Date: 2024-02-03,  Writer: 박태현
@ Explain
- NetworkBaseController로 상속받아서 사용할 Server와 통신 클래스
*/
class UNetworkBaseController abstract : public UBase {
public:
	UNetworkBaseController();
	DESTRUCTOR(UNetworkBaseController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) PURE;
	void SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize);
protected:
	void ServerTick();
	virtual void NativePacket() PURE;
	void RecvPacketCombine(UOverExp* _pOverExp, _llong _numBytes);
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) PURE;
	void RecvTcpPacket();
	/*
	@ Date: 2023-01-05, Writer: 박태현
	@ Explain
	- ProtocolBuffer를 TCP에 연결된 상대에게 보내기 위한 템플릿 함수이다.
	*/
	template<class T>
		requires CheckProtoType<T>
	void SendProtoData(const T& _data, short _tag)
	{
		static BUFFER Buffer;
		_data.SerializePartialToArray((void*)&Buffer[0], static_cast<int>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		SendTcpPacket(&Buffer[0], _tag, size);
	}
protected: /* get set */
	const SOCKET& GetClientTcpSocket() const { return m_ClientTcpSocket; }
	const SOCKET& GetClientUdpSocket() const { return m_ClientUdpSocket; }
	CSHPTRREF<UNetworkAddress> GetNetworkAddress() const { return m_spNetworkAddress; }
private:
	static void ServerThread(void* _pData);
private:
	virtual void Free() override;
private:
	_bool													m_isNetworkTickRunning;
	HANDLE												m_IocpHandle;
	UOverExp											m_RecvTcpOverExp;
	SOCKET												m_ClientTcpSocket;
	SOCKET												m_ClientUdpSocket;
	WSADATA											m_WsaData{};

	TOTALBUFFER									m_TcpTotalBuffer;
	_llong													m_RemainBufferLength;

	SHPTR< UNetworkAddress>			m_spNetworkAddress;
};

END