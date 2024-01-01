#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H

namespace Core
{
	enum
	{
		TCP_PORT_NUM = 8080,
		UDP_PORT_NUM = 8081,
		PACKET_SIZE_CNT = 2
	};

	/*
	@ Data: 2023-12-22
	@ Writer: 박태현
	@ Explain
	- Packet의 앞쪽 부분을 담당하는 Head 부분이다.
	*/
	struct PACKETHEAD {
		short		PacketSize;
		short		PacketType;

		PACKETHEAD() : PacketSize{ 0 }, PacketType{ 0 } {}
		PACKETHEAD(const short _size, const short _type) : PacketSize(_size), PacketType(_type) {}
	};

	static const std::string IP_ADDRESS = "127.0.0.1";
	constexpr static unsigned int PACKETHEAD_SIZE{ sizeof(PACKETHEAD) };
}

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_COREPAKCET_H