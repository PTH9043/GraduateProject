#pragma once

#include "UBase.h"

BEGIN(Engine)

using BUFFER = ARRAY < _char, MAX_BUFFER_LENGTH>;

enum COMP_TYPE { OP_ACCEPT, OP_RECV, OP_SEND, OP_NPC_MOVE, OP_NPC_FIND, OP_NPC_ATTACK };

//class UOverExp {
//public:
//	UOverExp() {
//		m_wsaBuffer.len = MAX_BUFFER_LENGTH;
//		m_wsaBuffer.buf = &m_Buffer[0];
//		m_CompType = OP_RECV;
//		ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
//	}
//	UOverExp(_char* _Packet) {
//		m_wsaBuffer.len = MAX_BUFFER_LENGTH;
//		m_wsaBuffer.buf = &m_Buffer[0];
//		m_CompType = OP_SEND;
//		ZeroMemory(&m_Over, sizeof(WSAOVERLAPPED));
//		memcpy(&m_Buffer[0], _Packet, packet[0]);
//	}
//
//private:
//	WSAOVERLAPPED		m_Over;
//	WSABUF						m_wsaBuffer;
//	BUFFER							m_Buffer;
//	COMP_TYPE					m_CompType;
//};


END

