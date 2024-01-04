#include "ClientDefines.h"
#include "CClientSession.h"
#include "UService.h"

CClientSession::CClientSession(OBJCON_CONSTRUCTOR, TCPSOCKET _TcpSocket, Core::SHPTR<Core::UService> _spService, SESSIONID _ID)
	: Core::USession(OBJCON_CONDATA, std::move(_TcpSocket), _spService, _ID, Core::SESSIONTYPE::PLAYER)
{
}

_bool CClientSession::Start()
{
	return __super::Start();
}

void CClientSession::ReadData()
{
	__super::ReadData();
}

_bool CClientSession::WriteData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
{
	CombineSendBuffer(_pPacket, _PacketHead);
	_bool result{ true };

	TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);
	Socket.async_write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE),
		[&](const boost::system::error_code& _error, std::size_t _Size) {
			SESSIONID SessionID = GetID();
			Core::SHPTR<Core::UService> Service{ GetService() };
			// Packet
			if (_error)
			{
				if (nullptr != Service)
					Service->RemoveService(SessionID);
				result = false;
			}
			else
			{
				SendMsg();
			}
		});
	return result;
}

void CClientSession::Disconnect()
{		
	// Remove Object 조합 
	SC_REMOVE_OBJECT scRemoveObject = PROTOFUNC::CreateScRemoveObject(GetID());
	Core::UBuffer Buffer;
	Core::PACKETHEAD PacketHead;
	CombineProto<SC_REMOVE_OBJECT>(Buffer, PacketHead, scRemoveObject, TAG_SC_LOGOUT);

	WriteData(&Buffer.GetBuffer()[0], PacketHead);
}

void CClientSession::ConnectTcpSocket()
{
	TCPSOCKET& TcpSocket = GetTcpSocket(REF_RETURN);
	TcpSocket.async_connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string(IP_ADDRESS),
		Core::TCP_PORT_NUM), [&](const boost::system::error_code& _error) {
			SHPTR<UService> spService = GetService();
			SESSIONID ID = GetID();
			// 만약 연결 실패했으면 제거
			if (_error)
			{
				spService->RemoveService(ID);
			}
			else
			{
				SendMsg();
			}
		});
}

void CClientSession::SendMsg()
{
	static std::vector<std::string> wordList = {
	"apple", "banana", "chocolate", "dog", "elephant",
	"flower", "guitar", "happiness", "internet", "jazz"
	};

	CS_LOGIN Login;
	Login.set_user_name(wordList[rand() % wordList.size()]);
	Core::UBuffer Buffer;
	Core::PACKETHEAD PacketHead;
	CombineProto<CS_LOGIN>(REF_OUT Buffer, REF_OUT PacketHead, Login, TAG_CS_LOGIN);
	WriteData(&Buffer.GetBuffer()[0], PacketHead);
}

_bool CClientSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
{
	switch (_PacketHead.PacketType)
	{
	case TAG_SERVER::TAG_SC_LOGIN:
	{
		SC_CHECKLOGIN Login;
		Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));

		ReadData();
	}
		break;
	}
	return true;
}

void CClientSession::Free()
{
}
