
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
	void MakePlayerState(PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, bool _animstate, bool _movespeed, 
		double _animDuration, int _curAnimIndex);
	void MakeSelfPlayerMove(SELFPLAYERMOVE* _pOut, LLONG _id, const VECTOR3& _move);
	void MakeCharMove(CHARMOVE* _pOut, LLONG _id, const VECTOR3&  _move, const VECTOR3& _rotate);

	/* =========== SC =============== */
	// Server To Client 

	void MakeScConnectSuccess(SC_CONNECTSUCCESS* _pOut, LLONG _id,  int _cellIndex,  int _type);
	void MakeScOtherClientLogin(SC_OTHERCLIENTLOGIN* _pOut, LLONG _id, int _cellIndex, int _type);
	void MakeScMoveFailed(SC_MOVEFAILED* _pOut, LLONG _id, VECTOR3* _pPrevPos);
	void MakeScViewInRange(SC_VIEWINRANGE* _pOut, LLONG _id, VECTOR3* _pPos, int _cellIndex, int _type);
	void MakeScStartInformationSucess(SC_START_INFORMATION_SUCCESS* _pOut, LLONG _id, int _monsterNum);
	void MakeScMonsterResourceData(SC_MONSTERRESOURCEDATA* _pOut, LLONG _id, const VECTOR3& _vPos,
		const VECTOR3& _vRotate, const VECTOR3& _vScale, int _animIndex, int _type);
	void MakeScMonsterState(SC_MONSTERSTATE* _pOut, LLONG _id, const double& _dAnimTime, int _animindex, int _state);
	void MakeScMonsterStateHavePos(SC_MONSTERSTATEHAVEPOS* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate, 
		const double& _dAnimTime, int _animindex, int _state);

	/* =========== CS =============== */
	// Client To Server 

	void MakeCsLogin(CS_LOGIN* _pOut, LLONG _id);
	void MakeCsAttack(CS_ATTACK* _pOut, LLONG _id, float _damage, VECTOR3* _pMovePos);
	void MakeCsDisconnect(CS_DISCONNECT* _pOut, LLONG _id);
	void MakeScResourceReceiveSuccess(CS_RESOURCE_RECEIVE_SUCCES* _pOut, LLONG _id);

	class PROTOCOL_MEMORY_LEAK_REMOVER
	{
	public:
		PROTOCOL_MEMORY_LEAK_REMOVER(){}
		~PROTOCOL_MEMORY_LEAK_REMOVER();
	};
}

static PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER MemoryRemover;