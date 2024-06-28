#include <string>
#include "ProtocolBuffer.h"
#include <thread>


PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER::~PROTOCOL_MEMORY_LEAK_REMOVER()
{
	google::protobuf::ShutdownProtobufLibrary();
}

void PROTOFUNC::MakeVector3(VECTOR3* _pOut, float _x, float _y, float _z)
{
	assert(nullptr != _pOut);
	_pOut->set_x(_x);
	_pOut->set_y(_y);
	_pOut->set_z(_z);
}

void PROTOFUNC::MakeVector4(VECTOR4* _pOut, float _x, float _y, float _z, float _w)
{
	assert(nullptr != _pOut);
	_pOut->set_x(_x);
	_pOut->set_y(_y);
	_pOut->set_z(_z);
	_pOut->set_w(_w);
}

void PROTOFUNC::MakeEqInfo(EQINFO* _pOut, float _gold, float _increasePower, float _increaseDefensive, float _increaseHp)
{
	assert(nullptr != _pOut);
	_pOut->set_gold(_gold);
	_pOut->set_increasepower(_increasePower);
	_pOut->set_increasedefensive(_increaseDefensive);
	_pOut->set_increasehp(_increaseHp);
}

void PROTOFUNC::MakePlayerState(PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, bool _animstate, 
	bool _movespeed, double _animDuration, int _curAnimIndex)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_animationtime(_animDuration);
	_pOut->set_animationindex(_curAnimIndex);
}

void PROTOFUNC::MakePlayerState(PLAYERSTATE* _pOut, LLONG _id, bool _ifattack,
	 bool _animstate, bool _movespeed, double _animDuration, int _curAnimIndex)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_animationtime(_animDuration);
	_pOut->set_animationindex(_curAnimIndex);
}

void PROTOFUNC::MakePlayerAnimState(PLAYERANIMSTATE* _pOut, LLONG _id, int animstate, bool ifattack,
	bool isRunshift, bool isMoveFront, bool isMoveBack, bool isMoveLeft, bool isMoveRight, bool isWAttack, 
	bool isSAttack, bool isRAttack, bool isCombo, bool isRoll, bool isHit, bool isJump, int wComboStack, int sComboStack)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_animstate(animstate);
	_pOut->set_ifattack(ifattack);
	_pOut->set_isrunshift(isRunshift);
	_pOut->set_ismovefront(isMoveFront);
	_pOut->set_ismoveback(isMoveBack);
	_pOut->set_ismoveleft(isMoveLeft);
	_pOut->set_ismoveright(isMoveRight);
	_pOut->set_iswattack(isWAttack);
	_pOut->set_issattack(isSAttack);
	_pOut->set_israttack(isRAttack);
	_pOut->set_iscombo(isCombo);
	_pOut->set_isroll(isRoll);
	_pOut->set_ishit(isHit);
	_pOut->set_isjump(isJump);
	_pOut->set_wcombostack(wComboStack);
	_pOut->set_scombostack(sComboStack);
}

void PROTOFUNC::MakeSelfPlayerMove(SELFPLAYERMOVE* _pOut, LLONG _id, const VECTOR3& _move)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_movex(_move.x());
	_pOut->set_movey(_move.y());
	_pOut->set_movez(_move.z());
}

void PROTOFUNC::MakeCharMove(CHARMOVE* _pOut, LLONG _id, const VECTOR3& _move, 
	const VECTOR4& _rotate, bool _jumpingstate)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_movex(_move.x());
	_pOut->set_movey(_move.y());
	_pOut->set_movez(_move.z());

	_pOut->set_rotatex(_rotate.x());
	_pOut->set_rotatey(_rotate.y());
	_pOut->set_rotatez(_rotate.z());
	_pOut->set_rotatew(_rotate.w());
	_pOut->set_jumpingstate(_jumpingstate);
}

/* =========== SC =============== */
// Server To Client 

void PROTOFUNC::MakeScConnectSuccess(SC_CONNECTSUCCESS* _pOut, LLONG _id, int _cellIndex,  int _type)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_cellindex(_cellIndex);
	_pOut->set_type(_type);
}

void PROTOFUNC::MakeScOtherClientLogin(SC_OTHERCLIENTLOGIN* _pOut, LLONG _id, int _cellIndex, int _type)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_cellindex(_cellIndex);
	_pOut->set_type(_type);
}

void PROTOFUNC::MakeScMoveFailed(SC_MOVEFAILED* _pOut, LLONG _id, VECTOR3* _pPrevPos)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_allocated_prevpos(_pPrevPos);
}

void PROTOFUNC::MakeScViewInRange(SC_VIEWINRANGE* _pOut, LLONG _id, VECTOR3* _pPos, int _cellIndex, int _type)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_allocated_position(_pPos);
	_pOut->set_cellindex(_cellIndex);
	_pOut->set_type(_type);
}

/* =========== CS =============== */
// Client To Server 

void PROTOFUNC::MakeCsLogin(CS_LOGIN* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
}


void PROTOFUNC::MakeCsAttack(CS_ATTACK* _pOut, LLONG _id, float _damage, VECTOR3* _pMovePos)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_damage(_damage);
	_pOut->set_allocated_attackpos(_pMovePos);
}

void PROTOFUNC::MakeCsDisconnect(CS_DISCONNECT* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
}
