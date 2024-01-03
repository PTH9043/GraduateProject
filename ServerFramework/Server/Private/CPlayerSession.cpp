#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "UService.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::USession(SESSION_CONDATA)
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
		// Buffer의 길이는 Packet의 길이 + PACKETHEAD의 길이이다.
		size_t BufferLength = static_cast<size_t>(_PacketHead.PacketSize + Core::PACKETHEAD_SIZE);
		SESSIONID SessionID = GetID();
		Core::SHPTR<Core::UService> Service{ GetService() };
		_bool result{ true };

		RunSendFunc([&BufferLength, &SessionID, &result, &_PacketHead, &Service]
		(const boost::system::error_code& _error, std::size_t _Size){
				// Packet
				if (!_error)
				{
					Service->RemoveService(SessionID);
				}
				else
				{
#ifdef USE_DEBUG
					if (_Size != BufferLength)
					{
						std::cout << "Packet Send Failed [" << SessionID << "] EC [" << _error << "]\n";
					}
#endif
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

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		return _bool();
	}

	void CPlayerSession::Free()
	{
	}
}
