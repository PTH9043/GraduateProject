#pragma once

#include "ServerUtility.h"

BEGIN(Engine)

class UNetworkBaseController final : public UBase {
public:
	UNetworkBaseController();
	DESTRUCTOR(UNetworkBaseController)
public:
	virtual HRESULT NativeConstruct(const _wstring& _wstrIPAddress, const _int _PortNumber) PURE;
protected:
	void ServerTick();
	virtual void ProcessPacket(_char* _pPacket, _llong _Size) PURE;
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
	SHPTR< UNetworkAddress>			m_spNetworkAddress;
};

END