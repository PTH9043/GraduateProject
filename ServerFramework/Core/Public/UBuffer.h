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
	using BUFF = ARRAY<_char, MAX_BUFFER_LENGTH>;

public:
	UBuffer();
	UBuffer(MOVE UBuffer&& _Other);
	UBuffer(const UBuffer& _rhs);
	UBuffer(_char* const _Buffer, const _llong& _TotalBuffer);
	~UBuffer();

	UBuffer& operator=(UBuffer&& _Other);
public:
	// 버퍼를 다시 담아 놓는다. 
	void BufferReceive( _char* const _Buffer,  const _llong& _TotalBuffer);
	// Combines Buffer 
	void CombineBuffers( _char* const _DefiniationBuffer, REF_OUT _llong& _CurrentBufferLocation);
public:
	 BUFF& GetBuffer() { return m_Buff; }
	const _llong GetBufferLength() const { return m_BufferLength; }
private:
	BUFF		m_Buff;
	_llong		m_BufferLength;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UBUFFER_H