#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UROOM_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UROOM_H

#include "UObject.h"

BEGIN(Core)
class USession;
/*
@ Date: 2023-01-10, Writer: 박태현
@ Explain
- 각 프레임워크에서 재정의하는 Room 클래스로 해당 룸에서 세션들을 받아서 해당 세션들이 얼마나 들어왔는지 
확인하고 해당 룸에서 게임에 대한 처리를 수행한다. 
*/
class CACHE_ALGIN_CORE_DLL URoom abstract : public UObject {
public:
	using SESSIONCONTAINER = CONHASHMAP<SESSIONID, SHPTR<USession>>;
public:
	URoom(OBJCON_CONSTRUCTOR, _uint _MaxSessionCount);
	DESTRUCTOR(URoom)
public:
	virtual _bool NativeConstruct() PURE;

	_bool InsertSession(SHPTR<USession> _spSession);
	SHPTR<USession> FindSession(const SESSIONID _SessionID);
	void DeleteSession(const SESSIONID _SessionID);
	void BroadCastMessage(char* _pPacket, PACKETHEAD _PacketHead);
public: /* Get Set */
	_uint GetMaxSessionCount() const { return m_MaxSessionCount; }
	_bool IsEmptyRoom() const { return m_isEmptyRoom.load(); }
private:
	virtual void Free() override;
private:
	_uint									m_MaxSessionCount;
	ATOMIC<_bool>				m_isEmptyRoom;
	SESSIONCONTAINER		m_SessionContainer;
};

END

#endif 

