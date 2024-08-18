#include "EngineDefine.h"
#include "UNetworkSender.h"
#include "UNetworkBaseController.h"
#include "ServerUtility.h"

UNetworkSender::UNetworkSender(SHPTR<UNetworkBaseController> _spNetworkBaseController) :
	m_pClientTcpSocket{NULL},  m_pOverExp{nullptr}
{
	m_pClientTcpSocket = _spNetworkBaseController->GetClientSocketPointer();
	m_pOverExp = Make::xnew<UOverExp>();
}
void UNetworkSender::SendProtoPacket(_char* _pPacket, _short _PacketType, _short _PacketSize)
{
	m_pOverExp->SendBufferReady(_pPacket, _PacketType, _PacketSize);
	UServerMethods::SendTcpPacket(*m_pClientTcpSocket, m_pOverExp);
}

void UNetworkSender::Free()
{
	Make::xdelete(m_pOverExp);
}
