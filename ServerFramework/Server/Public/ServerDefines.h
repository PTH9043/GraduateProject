#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H

#pragma warning (disable : 4244)
#pragma warning (disable : 4018)

#include "CoreDefines.h"
#include "ProtocolBuffer.h"

#ifdef _DEBUG
#pragma comment(lib, "ProtocolBufferD")
#pragma comment(lib, "CoreD")
#else
#pragma comment(lib, "ProtocolBuffer")
#pragma comment(lib, "Core")
#endif

#define DEFINE_STATIC_CONSTCHAR(Name, Define) static const char* Name = Define;

using namespace Core;

namespace Server 
{
	enum
	{
		ENABLE_KEY = 0x01,
		KEYBOARD_F = 0x10,
	};

	enum TIMERTYPE : Core::_int
	{
		TIMER_ASTAR, TIMER_DB, TIMER_MOB, TIMER_END
	};

	enum SARCOPHAGUSTYPE {
		SARCO_STANDING, SARCO_LAYING, SARCO_END
	};

	enum MUMMYTYPE {
		MUMMY_STANDING, MUMMY_LAYING, MUMMY_END
	};

	struct MOBDATA {
		Core::_string			strOwnerName;
		Core::_string			strAnimModelName;
		Core::_string			strAnimName;
		Core::_float4x4		mWorldMatrix;
		_int								iMobID;
		_string						strRoomName;

		MOBDATA() : strOwnerName{""}, strAnimModelName{""}, strAnimName{""}, mWorldMatrix{Core::_float4x4::Identity},
			strRoomName{ "" } {}
	};

	struct MOBSERVERDATA
	{
		Core::_int				iStartAnimIndex;
		Core::_int				iMobType;
		Core::_float4x4	mWorldMatrix;
		Core::_int				iMobID;
		Core::_int				iMobParentsID;
		_string					strRoomName;

		MOBSERVERDATA() : iStartAnimIndex{ 0 }, iMobType{ 0 }, mWorldMatrix{ Core::_float4x4::Identity }, iMobID{ 0 }, iMobParentsID{ 0 },
			strRoomName{ "" } {}
	};

	struct STATICOBJSERVERDATA
	{
		Core::_float4x4	mWorldMatrix;
		Core::_int				iServerID;
		Core::_int				iObjType;
		_string					strRoomName;

		STATICOBJSERVERDATA() : mWorldMatrix{ Core::_float4x4::Identity }, iServerID{ 0 }, iObjType{ 0 }, strRoomName{ "" } {}
	};

	enum COMMONSTATE
	{
		ANIM_IDLE, ANIM_MOVE, ANIM_ATTACK, ANIM_DEATH, ANIM_HIT
	};
	enum MUMMYANIMSTATE
	{
		MUMMY_ANIM_SLEEP = 6, MUMMY_ANIM_AWAKE = 7, MUMMY_ANIM_TAUNT = 8
	};

	enum COLLIDERTYPE
	{
		COLLIDER_MAIN,
		COLLIDER_FORINTERACTION_BARS,
		COLLIDER_FORINTERACTION_COREANUBIS,
		COLLIDER_FORINTERACTION_COREMINOTAUR,
		COLLIDER_FORINTERACTION_COREHARLEQUIN,
		COLLIDER_FORINTERACTION_STATUE,
		COLLIDER_FORATTACK,
		COLLIDER_MAGICSPHERE,
		COLLIDER_MAGICCIRCLE,

		COLLIDER_END
	};
}

#define CREATED_SERVERMOBDATA

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_SERVERDEFINES_H