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
	virtual HRESULT NativeConstruct(const _wstring& _wstrIPAddress, const _int _PortNumber) PURE;
protected:
	void ServerTick();
	void TcpPacketRecv(UOverExp* _pOverExp, _llong _numBytes);
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) PURE;
	void SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize);
	void RecvTcpPacket();
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