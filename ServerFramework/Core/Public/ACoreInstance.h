#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

#include "ACoreBase.h"

BEGIN(Core)
class AService;
class AThreadManager;
class ARandomManager;
class ALogger;
class ASpaceManager;
class ASession;
class AMySqlDriver;
class AGameObject;
class ANavigation;
class APathFinder;
class ATransform;
class ACell;

using NAVIGATIONWORKBENCH = ARRAY<SHPTR<ANavigation>, TLS::MAX_JOBTHREAD>;
using PATHFINDERWORKBENCH = ARRAY<SHPTR<APathFinder>, TLS::MAX_JOBTHREAD>;

using SESSIONCONTAINER = CONUNORMAP<SESSIONID, SHPTR<ASession>>;
using GAMEOBJECTCONTAINER = CONUNORMAP<SESSIONID, SHPTR<AGameObject>>;
/*
@ Date: 2024-01-23
@ Writer: 박태현
@ Explain
- 생성, 소멸자를 이용해서 서버가 종료될 때, 메모리가 제대로 해제되게 하기 위한 클래스이다. 
무조건 한 개만 생성한다. 
*/
class CORE_DLL ACoreInstance : public ACoreBase {
public:
	ACoreInstance();
	NO_COPY(ACoreInstance)
	DESTRUCTOR(ACoreInstance)
public:
	void ReadyCoreInstance(SHPTR<AService> _spService);
	_bool Start();
public: /* Service */
	// ID를 통해서 SessionID
	 SHPTR<ASession> FindSession(const SESSIONID _SessionID);
	// 전체 서버 참여자에게 메시지를 보내는 함수이다. 
	 void BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead);
	 // 해당 Session ID를 제외한 전체 서버 참여자에게 메시지를 보내는 함수
	 void BroadCastMessageExcludingSession(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead);
	 // Direct로 메시지 보내는 함수
	 void DirectSendMessage(const SESSIONID _SessionID, _char* _pPacket, const PACKETHEAD& _PacketHead);
	// Session의 TCP Socket 연결만 끊고 Insert나 기타 다른 함수들을 정의해서 Socket들 제거
	 void LeaveService(const SESSIONID _SessionID);
	// Session을 Container에 저장하는 함수이다. 
	 void InsertSession(SESSIONID _SessionID, SHPTR<ASession> _spSession);
	 // Get 
	 const SESSIONCONTAINER& GetSessionContainer() const;
	 const GAMEOBJECTCONTAINER& GetGameObjectContainer() const;
public: /* ThreadManager */
	void RegisterFunc(const THREADFUNC& _CallBack, void* _Data);
	void Join();
public: /* RandomManager */
	_int		ReturnRadomNumber(const _int _iMinNum, const _int _iMaxNum);
	_int		ReturnRadomNumber(const _int _iMaxNum);
public: /* SpaceManager */
	void BuildGameSpace(const SPACEINFO& _SpaceInfo);
public: /* MySqlDriver */
	// ExcuteQueryMessage
	_bool ExcuteQueryMessage(SQLTABLETYPE _TableType, SQLQUERYTYPE _sqlQueryType, const _string& _strQueryData);

	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _bool _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _float _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _double _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _short _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _int _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, _llong _Value);
	void BindParam(SQLTABLETYPE _TableType, _int _ParamIndex, const _string& _Value);
public: /* Navigation*/
	// Compute Height
	const _float  ComputeHeight(const Vector3& _vPosition);
	void ComputeHeight(CSHPTRREF<ATransform> _spTransform);
	// Move
	_bool IsMove(Vector3 _vPosition, SHPTR<ACell>& _spCell);
public: /* PathFinder */
	// Find Path
	void FindPath(Vector3 _vStartPos, Vector3 _vEndPos);
	LIST<Vector3>& GetBestList();
private:
	virtual void Free() override;
private:
	MUTEX											m_Mutex;
	SHPTR<AService>						m_spService;
	SHPTR<AThreadManager>		m_spThreadManager;
	SHPTR<ARandomManager>		m_spRandomManager;
	SHPTR<ASpaceManager>			m_spSpaceManager;
	SHPTR< AMySqlDriver>				m_spMySqlDriver;

	NAVIGATIONWORKBENCH			m_NavigationWorkBench;
	PATHFINDERWORKBENCH			m_PathFinderWorkBench;
};

END


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_ACOREINSTANCE_H

