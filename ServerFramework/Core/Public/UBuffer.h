#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H

BEGIN(Core)

/*
@ Date: 2023-12-31
@ Writer: 박태현
@ Explain
- 클라이언트에서 버퍼를 받아서 저장하는 클래스이다. 
*/
class CACHE_ALGIN_CORE_DLL UBuffer {
	using RECVBUFF = ARRAY<_char, MAX_BUFFER_LENGTH>;

public:
	UBuffer();
	UBuffer(MOVE UBuffer&& _Other);
	UBuffer(_char* const _Buffer, const _uint& _TotalBuffer);
	~UBuffer();

	UBuffer& operator=(UBuffer&& _Other);
public:
	// 버퍼를 다시 담아 놓는다. 
	void BufferReceive( _char* const _Buffer,  const _uint& _TotalBuffer);
	// Combines Buffer 
	void CombineBuffers( _char* const _DefiniationBuffer, REF_OUT _int& _CurrentBufferLocation);
public:
	const RECVBUFF& GetRecvBuffer() const { return m_RecvBuff; }
	const _uint GetBufferLength() const { return m_BufferLength; }
public:
	RECVBUFF	m_RecvBuff;
	_uint				m_BufferLength;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H