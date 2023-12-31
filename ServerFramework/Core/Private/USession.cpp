#include "CoreDefines.h"
#include "USession.h"
#include "UBuffer.h"

namespace Core
{
	USession::USession(MOVE Asio::ip::tcp::socket&& _TcpSocket, SESSIONTYPE _SessionType, ID _ID) : 
		m_TcpSocket(std::move(_TcpSocket)), m_SessionType(_SessionType), m_ID(_ID), m_CurBuffuerLocation{0}
	{
	}

	_bool USession::Start()
	{
		ReadData();
		return true;
	}

	_bool USession::ReadData()
	{
		_bool result{ true };
		m_TcpSocket.async_read_some(Asio::buffer(m_RecvBuffer),
			[&](const boost::system::error_code& _error, std::size_t _Size)
			{
				if (!_error)
				{
					// Insert Buffer Queue
					UBuffer Buf(m_RecvBuffer.data(), (_uint)(_Size));
					// Packet Combine 
					PacketCombine(std::move(Buf));
					ZeroMemory(&m_RecvBuffer, MAX_BUFFER_LENGTH);
					// Read Data
					ReadData();
				}
				else
				{
#ifdef USE_DEBUG
					if (_error.value() == boost::asio::error::operation_aborted) return;
					std::cout << "Receive Error on Session[" << m_ID << "] EC[" << _error << "]\n";
#endif
					result = false;
				}
			});
		return result;
	}

	_bool USession::WriteData(_char* _Packet, const PACKETHEAD& _PacketHead)
	{
		_bool result{ true };
		CombineSendBuffer(m_SendBuffer, _Packet, _PacketHead);
		// Buffer의 길이는 Packet의 길이 + PACKETHEAD의 길이이다.
		size_t BufferLength = static_cast<size_t>(_PacketHead.PacketSize + PACKETHEAD_SIZE);
		m_TcpSocket.async_write_some(Asio::buffer(m_SendBuffer, BufferLength),
			[&](const boost::system::error_code& _error, std::size_t _Size)
			{
				// Packet
				if (!_error)
				{
#ifdef USE_DEBUG
					if (_Size == _PacketHead.PacketSize)
					{
						std::cout << "Packet Send Failed [" << m_ID << "] EC [" << _error << "]\n";
					}
#endif
					result = false;
				}
			});
		return result;
	}

	void USession::End()
	{
	}

	/*
	===========
	PUBLIC
	===========
	PROTECTED
	===========
	*/

	/*
	@ Data: 2023-12-31
	@	 Writer : 박태현
	@ Explain
	- Buffer 클래스를 받아서 Combine 한 후 Packet을 처리하는 함수이다.
	*/
	void USession::PacketCombine(MOVE UBuffer&& _Buffer)
	{
		// 버퍼를 조합한다. 
		_Buffer.CombineBuffers(&m_RecvBuffer[0], m_CurBuffuerLocation);
		short moveBuffer{ 0 };
		char* pBufferMove = &m_RecvBuffer[0];
		// 만약 BufferLocation이 존재할 때
		while (m_CurBuffuerLocation > 0)
		{
			// PacketSize를 구한다. 
			PACKETHEAD PacketHead;
			memcpy(&PacketHead, pBufferMove, PACKETHEAD_SIZE);
			short CurrPacket = PacketHead.PacketSize + PACKETHEAD_SIZE;
			// 패킷의 현재 위치가 음수가 되는 경우면 
			if (m_CurBuffuerLocation - CurrPacket < 0)
			{
				// 전체 버퍼값에서 이동한 Packet 사이즈를 뺀 후 
				_uint EraseValue = MAX_PROCESSBUF_LENGTH - moveBuffer;
				// 최종적인 버퍼에 PacketSize만큼 이동하여 앞쪽에 존재하는 데이터들을 지운다. 
				memcpy(&m_RecvBuffer[0], pBufferMove, EraseValue);
				return;
			}
			ProcessPacket(&pBufferMove[PACKETHEAD_SIZE], PacketHead);
			// Buffer의 위치를 옮긴다. 
			m_CurBuffuerLocation -= CurrPacket;
			pBufferMove += CurrPacket;
			moveBuffer += CurrPacket;
		}
		m_CurBuffuerLocation = 0;
		ZeroMemory(&m_RecvBuffer, MAX_PROCESSBUF_LENGTH);
	}

	/*
	@ Data: 2023-12-31
	@	 Writer : 박태현
	@ Explain
	- Buffer를 조합하는 함수
	*/
	void USession::CombineSendBuffer(REF_OUT BUFFER& _SendBuffer, _char* _Packet, const PACKETHEAD& _PacketHead)
	{
		ZeroMemory(&_SendBuffer[0], MAX_BUFFER_LENGTH);
		memcpy(&_SendBuffer[0], &_PacketHead, PACKETHEAD_SIZE);
		// [0 ~ 1] : PACKETSIZE		[2 ~ 3] : PACKETTYPE		[4 ~ ] Remain... 
		memcpy(&_SendBuffer[PACKETHEAD_SIZE], _Packet, _PacketHead.PacketSize);
	}

	void USession::Free()
	{
	}

}