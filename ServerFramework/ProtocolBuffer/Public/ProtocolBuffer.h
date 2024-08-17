#ifndef _SERVERFRAMEWORK_PROTOCOLBUFFER_H
#define _SERVERFRAMEWORK_PROTOCOLBUFFER_H


#include "Protocol.pb.h"
#include "Tag.pb.h"

/*
@ Date: 2024-06-19, Writer: 박태현
@ Explain: Protocol Buffer가 활성화 되었는지에 대한 여부
*/
#define _ENABLE_PROTOBUFF

/*
@ Date: 2024-06-19, Writer: 박태현
@ Explain: ProtocolBuffer를 할당하기 위한 객체.
*/
#define PROTOALLOC(Type, Name) Type* Name = new Type();

namespace PROTOFUNC
{

	using LLONG =  long long;

	void MakeVector3(VECTOR3* _pOut, float _x, float _y, float _z);
	void MakeVector4(VECTOR4* _pOut, float _x, float _y, float _z, float _w);
	void MakeEqInfo(EQINFO* _pOut,  float _gold, float _increasePower, float _increaseDefensive, float _increaseHp);
	void MakeCollisionData(COLLISIONDATA* _pOut, LLONG _id, LLONG _enemyID);
	void MakeCharState(CHARSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate,
		int _animstate, int _curAnimIndex, int _triggerOn);
	void MakeCharState(CHARSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate);
	void MakeMobState(MOBSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate,
		int _animstate, int _curAnimIndex, bool _triggerOn, bool _foundOn, bool _damageOn, double _animtime);

	/* =========== SC =============== */
	// Server To Client 

	void MakeScConnectSuccess(SC_CONNECTSUCCESS* _pOut, LLONG _id,  int _cellIndex,  int _type);
	void MakeScOtherClientLogin(SC_OTHERCLIENTLOGIN* _pOut, LLONG _id, int _cellIndex, int _type);
	void MakeScMonsterFind(SC_MONSTERFIND* _pOut, LLONG _id, int _targetID);
	void MakeScDamaged(SC_DAMAGED* _pOut, LLONG _id, float _hp);
	void MakeScSeePlayerMove(SC_SEEPLAYERMOVE* _pOut, LLONG _id, const VECTOR3& _vPos);
	void MakeScHarlequinThrowing(SC_HARLEQUINTHROWING* _pOut, LLONG _id, const VECTOR3& _vPos,
		int _throwenable, int _index);
	void MakeScStaticObjFind(SC_STATICOBJFIND* _pOut, LLONG _id, int _enable);
	void MakeScAnubisState(SC_ANUBISSTATE* _pOut, LLONG _id, int _shieldOn, float _ShieldTimer);
	void MakeScSavePointEnable(SC_SAVEPOINTENABLE* _pOut, LLONG _id, int _count);
	void MakeScPlayerGetUp(SC_PLAYERGETUP* _pOut, LLONG _id, float _hp,  const VECTOR3& _vPos, int _camCellIndex);
	void MakeScEnding(SC_ENDING* _pOut, LLONG _id);

	/* =========== CS =============== */
	// Client To Server 

	void MakeCsLogin(CS_LOGIN* _pOut, LLONG _id);
	void MakeCsAttack(CS_ATTACK* _pOut, LLONG _id, float _damage, VECTOR3* _pMovePos);
	void MakeCsDisconnect(CS_DISCONNECT* _pOut, LLONG _id);
	void MakeCsHeal(CS_HEAL* _pOut, LLONG _id);
	void MakeCsPlayerOrder(CS_PLAYERORDER* _pOut, LLONG _id, int _playerOrder);
	void MakeCsDamaged(CS_DAMAGED* _pOut, LLONG _id, float _Damaged);
	void MakeCsPressKey(CS_PRESSKEY* _pOut, LLONG _id, int _key);
	void MakeCsDamagedToMonster(CS_DAMAGEDTOMONSTER* _pOut, LLONG _id, float _damge, LLONG _enemyID);
	void MakeCsSavePointEnable(CS_SAVEPOINTENABLE* _pOut, LLONG _id, const VECTOR3& _vPos, int _camCellIndex);
	void MakeCsCoreEnable(CS_CORENABLE* _pOut, LLONG _id);

	class PROTOCOL_MEMORY_LEAK_REMOVER
	{
	public:
		PROTOCOL_MEMORY_LEAK_REMOVER(){}
		~PROTOCOL_MEMORY_LEAK_REMOVER();
	};
}

static PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER MemoryRemover;

#endif // _SERVERFRAMEWORK_PROTOCOLBUFFER_H