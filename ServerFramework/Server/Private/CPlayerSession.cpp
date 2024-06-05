#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER))
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
			Core::SHPTR<Core::ACoreInstance> spCoreInstance{ GetCoreInstance() };
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
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket(){}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		static thread_local BUFFER Buffer;
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		switch (_PacketHead.PacketType)
		{
		case TAG_CS::TAG_CS_LOGIN:
		{
			CS_LOGIN Login;
			Login.ParseFromArray(_pPacket, static_cast<_int>(_PacketHead.PacketSize));
			_llong value = CurrentMilliseconds() - Login.time_test();
			std::cout << Login.user_name() << std::endl;
		}
		{
			PACKETHEAD PacketHead;
			SC_CHECKLOGIN checkLogin;
			// 플레이어의 로그인이 확인되면 로그인 데이터를 넘겨준다. 
			checkLogin.set_id(GetSessionID());
			// ProtoData를 Combine 해준다. 
			CombineProto(REF_OUT Buffer, REF_OUT PacketHead, REF_OUT checkLogin, TAG_SC_LOGIN);
			spCoreInstance->BroadCastMessage(&Buffer[0], PacketHead);
		}
		break;
		// Logout 
		case TAG_CS::TAG_CS_LOGOUT:
		{
			PACKETHEAD PacketHead;
			CS_LOGOUT logout;
			logout.set_id(GetSessionID());
			// ProtoData를 Combine 해준다. 
			CombineProto(REF_OUT Buffer, REF_OUT PacketHead, REF_OUT logout, TAG_SC_LOGIN);
			spCoreInstance->BroadCastMessage(&Buffer[0], PacketHead);
			Disconnect();
			return false;
		}
		break;
		case TAG_CS::TAG_CS_MOVE:
		{
			CS_MOVE move;
			
		}
		break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
