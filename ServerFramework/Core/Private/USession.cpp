#include "CoreDefines.h"
#include "USession.h"
#include "UService.h"

namespace Core
{
	USession::USession(OBJCON_CONSTRUCTOR, MOVE TCPSOCKET _TcpSocket, SHPTR<UService> _spService, SESSIONID _ID, SESSIONTYPE _SessionType) :
		UObject(OBJCON_CONDATA),
		m_TcpSocket(std::move(_TcpSocket)), m_SessionType(_SessionType), m_ID(_ID), m_CurBuffuerLocation{0},
		m_wpService{_spService},
		m_isConnect{true}
	{
		MemoryInitialization(m_SendBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_RecvBuffer.data(), MAX_BUFFER_LENGTH);
		MemoryInitialization(m_TotalBuffer.data(), MAX_PROCESSBUF_LENGTH);
	}

	_bool USession::Start()
	{
		ReadData();
		return true;
	}

	void USession::ReadData()
	{
		RETURN_CHECK(false == m_isConnect, ;);
		m_TcpSocket.async_read_some(Asio::buffer(m_RecvBuffer),
			[this](const boost::system::error_code& _error, std::size_t _Size)
			{	
				if (!_error)
				{
					if (_Size == 0)
					{
						Leave();
						return;
					}
					PacketCombine(&m_RecvBuffer[0], _Size);
					ReadData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << m_ID << "] EC[" << _error << "]\n";
#endif
					Leave();
				}
			});
	}

	_bool USession::WriteData(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(false == m_isConnect, false);
		return true;
	}

	void USession::Disconnect()
	{
		m_isConnect = false;
	}

	void USession::ConnectTcpSocket()
	{
		m_TcpSocket.async_connect(Asio::ip::tcp::endpoint(Asio::ip::address::from_string(IP_ADDRESS),
			Core::TCP_PORT_NUM), [this](const boost::system::error_code& _error) {

				SESSIONID ID = m_ID;
				SHPTR<UService> spService = GetService(REF_RETURN);
				// 만약 연결 실패했으면 제거
				if (_error)
				{
#ifdef USE_DEBUG
					std::cout << "Connect Error on Session[" << m_ID << "] EC[" << _error << "]\n";
#endif
					spService->LeaveService(ID);
				}
			});
	}

	/*
	@ Data: 2024-01-04, Writer : 박태현
	@ Explain
	- Buffer 클래스를 받아서 Combine 한 후 Packet을 처리하는 함수이다.
	*/
	void USession::PacketCombine(_char* _pPacket, _llong _Size)
	{
		// 버퍼를 조합한다. 
		{
			::memcpy(&m_TotalBuffer[m_CurBuffuerLocation], _pPacket, _Size);
			m_CurBuffuerLocation +=_Size;
		}
		short moveBuffer{ 0 };
		char* pBufferMove = &m_TotalBuffer[0];
		// 만약 BufferLocation이 존재할 때
		while (m_CurBuffuerLocation > 0)
		{
			// PacketSize를 구한다. 
			PACKETHEAD PacketHead;
			memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
			short CurrPacket = PacketHead.PacketSize + PACKETHEAD_SIZE;
			// 패킷의 현재 위치가 음수가 되는 경우면 
			if ((m_CurBuffuerLocation - CurrPacket) < 0)
			{
				// 전체 버퍼값에서 이동한 Packet 사이즈를 뺀 후 
			//	_uint EraseValue = MAX_PROCESSBUF_LENGTH - moveBuffer;
				// 최종적인 버퍼에 PacketSize만큼 이동하여 앞쪽에 존재하는 데이터들을 지운다. 
				memcpy(&m_TotalBuffer[0], pBufferMove, m_CurBuffuerLocation);
				return;
			}
			ProcessPacket(&pBufferMove[PACKETHEAD_SIZE], PacketHead);
			// Buffer의 위치를 옮긴다. 
			m_CurBuffuerLocation -= CurrPacket;
			pBufferMove += CurrPacket;
			moveBuffer += CurrPacket;
		}
		m_CurBuffuerLocation = 0;
		MemoryInitialization(m_TotalBuffer.data(), MAX_PROCESSBUF_LENGTH);
	}

	/*
	@ Data: 2023-12-31,  Writer : 박태현
	@ Explain
	- Buffer를 조합하는 함수
	*/
	void USession::CombineSendBuffer( _char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		ZeroMemory(&m_SendBuffer[0], MAX_BUFFER_LENGTH);
		memcpy(&m_SendBuffer[0], &_PacketHead, PACKETHEAD_SIZE);
		// [0 ~ 1] : PACKETSIZE		[2 ~ 3] : PACKETTYPE		[4 ~ ] Remain... 
		memcpy(&m_SendBuffer[PACKETHEAD_SIZE], _pPacket, _PacketHead.PacketSize);
	}

	void USession::Leave()
	{
		SHPTR<UService> spService =m_wpService.lock();
		spService->LeaveService(m_ID);
	}

	void USession::Free()
	{
		m_TcpSocket.close();
	}

}