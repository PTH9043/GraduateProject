#include "CoreDefines.h"
#include "UBuffer.h"

namespace Core {

	UBuffer::UBuffer() : m_RecvBuff{}, m_BufferLength{0}
	{
		ZeroMemory(&m_RecvBuff, MAX_BUFFER_LENGTH);
	}

	UBuffer::UBuffer(MOVE UBuffer&& _Other) : m_RecvBuff{ _Other.m_RecvBuff}, m_BufferLength{ _Other.m_BufferLength}
	{
	}

	UBuffer::UBuffer(_char* const _Buffer, const _uint& _TotalBuffer) : m_RecvBuff{}, m_BufferLength{ _TotalBuffer }
	{
		ZeroMemory(&m_RecvBuff, MAX_BUFFER_LENGTH);
		::memcpy((_char*)&m_RecvBuff, _Buffer, static_cast<size_t>(m_BufferLength));
	}

	UBuffer::~UBuffer()
	{
	}

	UBuffer& UBuffer::operator=(UBuffer&& _Other)
	{
		m_RecvBuff = _Other.m_RecvBuff;
		m_BufferLength = _Other.m_BufferLength;
		return *this;
	}

	void UBuffer::BufferReceive(_char* const _Buffer, const _uint& _TotalBuffer)
	{
		ZeroMemory(&m_RecvBuff, MAX_BUFFER_LENGTH);
		m_BufferLength = _TotalBuffer;
		::memcpy((_char*)&m_RecvBuff, _Buffer, static_cast<size_t>(m_BufferLength));
	}

	void UBuffer::CombineBuffers(_char* const _pDefiniationBuffer, REF_OUT _int& _CurrentBufferLocation)
	{
		// 본래  버퍼에 현재 버퍼의 크기만큼 이동한 후, 받아온 packet 데이터를 이어붙인다. 
		::memcpy(_pDefiniationBuffer + _CurrentBufferLocation, m_RecvBuff.data(), m_BufferLength);
		// 현재 버퍼의 위치를 이동시킨다. 
		_CurrentBufferLocation += m_BufferLength;
	}

}