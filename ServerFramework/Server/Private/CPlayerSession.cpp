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
		RETURN_CHECK(false == IsConnect() , false);

		CombineSendBuffer( _pPacket, _PacketHead);
		_bool result{ true };

		TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);

		Socket.async_write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE),
			[this](const boost::system::error_code& _error, std::size_t _Size) {
			SESSIONID SessionID = GetID();
			Core::SHPTR<Core::UService> Service{ GetService() };
				// Packet
				if (_error)
				{
					if (nullptr != Service)
						Service->LeaveService(SessionID);
				}
				else
				{
					if (false == IsConnect())
					{
						if (nullptr != Service)
						{
							Service->LeaveService(SessionID);
						}
					}
				}
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		// Remove Object 조합 
		SC_REMOVE_OBJECT scRemoveObject = PROTOFUNC::CreateScRemoveObject(GetID());
		SendProtoData(scRemoveObject, TAG_SC_LOGOUT);
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket()
	{
		TCPSOCKET& TcpSocket = GetTcpSocket(REF_RETURN);
		TcpSocket.async_connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string(IP_ADDRESS),
			Core::TCP_PORT_NUM), [this](const boost::system::error_code& _error) {
				SESSIONID ID = GetID();
				SHPTR<UService> spService = GetService(REF_RETURN);
				// 만약 연결 실패했으면 제거
				if (_error)
				{
					if (nullptr != spService)
						spService->LeaveService(ID);
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
			std::cout << Login.user_name() << "\n";
		}
		//{
		//	// Remove Object 조합 
		//	SC_CHECKLOGIN scCheckLogin;
		//	scCheckLogin.set_id(GetID());
		//	SendProtoData(scCheckLogin, TAG_SC_LOGIN);
		//}
		break;
		case TAG_CS::TAG_CS_LOGOUT:
		{
			CS_LOGOUT logout;
			logout.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
			std::cout << _PacketHead.PacketSize << "\n";
	//		SHPTR<UService> spService = GetService();
		//	spService->LeaveService(logout.id());
			return false;
		}
		break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
