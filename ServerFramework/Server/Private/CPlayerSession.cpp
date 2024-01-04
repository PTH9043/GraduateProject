#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "UService.h"

namespace Server {

	CPlayerSession::CPlayerSession(OBJCON_CONSTRUCTOR, TCPSOCKET _TcpSocket, Core::SHPTR<Core::UService> _spService, SESSIONID _ID)
		: Core::USession(OBJCON_CONDATA, std::move(_TcpSocket), _spService, _ID, Core::SESSIONTYPE::PLAYER)
	{
	}

	_bool CPlayerSession::Start()
	{
		return __super::Start();
	}

	void CPlayerSession::ReadData()
	{
		__super::ReadData();
	}

	_bool CPlayerSession::WriteData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		CombineSendBuffer( _pPacket, _PacketHead);
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
				}
				else
				{
					result = false;
				}
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		// Remove Object 조합 
		SC_REMOVE_OBJECT scRemoveObject = PROTOFUNC::CreateScRemoveObject(GetID());
		Core::UBuffer Buffer;
		Core::PACKETHEAD PacketHead;
		CombineProto<SC_REMOVE_OBJECT>(Buffer, PacketHead, scRemoveObject, TAG_SC_LOGOUT);

		WriteData(&Buffer.GetBuffer()[0], PacketHead);
	}

	void CPlayerSession::ConnectTcpSocket()
	{
		TCPSOCKET& TcpSocket = GetTcpSocket(REF_RETURN);
		SESSIONID ID = GetID();
		SHPTR<UService> spService = GetService(REF_RETURN);
		CPlayerSession* pSession = this;
		TcpSocket.async_connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string(IP_ADDRESS),
			Core::TCP_PORT_NUM), [&spService, &ID, pSession](const boost::system::error_code& _error) {
				// 만약 연결 실패했으면 제거
				if (_error)
				{
					spService->RemoveService(ID);
				}
			});
	}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		switch (_PacketHead.PacketType)
		{
		case TAG_CS::TAG_CS_LOGIN:
		{
			CS_LOGIN Login;
			Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
		}
		{
			// Remove Object 조합 
			SC_CHECKLOGIN scCheckLogin;
			scCheckLogin.set_id(GetID());
			Core::UBuffer Buffer;
			Core::PACKETHEAD PacketHead;
			CombineProto<SC_CHECKLOGIN>(REF_OUT Buffer, REF_OUT PacketHead, scCheckLogin, TAG_SC_LOGIN);

			WriteData(&Buffer.GetBuffer()[0], PacketHead);
		}
			break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
