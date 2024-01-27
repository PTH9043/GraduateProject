#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "UCoreInstance.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::USession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER))
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
		RETURN_CHECK(false == IsConnected() , false);

		CombineSendBuffer( _pPacket, _PacketHead);
		_bool result{ true };

		TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);

		Socket.async_write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE),
			[this](const boost::system::error_code& _error, std::size_t _Size) {
			SESSIONID SessionID = GetSessionID();
			Core::SHPTR<Core::UCoreInstance> spCoreInstance{ GetCoreInstance() };
				// Packet
				if (_error)
				{
					if (nullptr != spCoreInstance)
						spCoreInstance->LeaveService(SessionID);
				}
				else
				{
					if (false == IsConnected())
					{
						if (nullptr != spCoreInstance)
						{
							spCoreInstance->LeaveService(SessionID);
						}
					}
				}
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		// Remove Object 조합 
		SC_REMOVE_OBJECT scRemoveObject = PROTOFUNC::CreateScRemoveObject(GetSessionID());
		SendProtoData(scRemoveObject, TAG_SC_LOGOUT);
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket(){}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		switch (_PacketHead.PacketType)
		{
		case TAG_CS::TAG_CS_LOGIN:
		{
			CS_LOGIN Login;
			Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
			_llong value = CurrentMilliseconds() - Login.time_test();
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
