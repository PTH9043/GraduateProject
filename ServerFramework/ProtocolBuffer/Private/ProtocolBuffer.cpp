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

void PROTOFUNC::MakeEqInfo(EQINFO* _pOut, float _gold, float _increasePower, float _increaseDefensive, float _increaseHp)
{
	assert(nullptr != _pOut);
	_pOut->set_gold(_gold);
	_pOut->set_increasepower(_increasePower);
	_pOut->set_increasedefensive(_increaseDefensive);
	_pOut->set_increasehp(_increaseHp);
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

void PROTOFUNC::MakeScPlayerState(SC_PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, 
	bool _jumpingstate, bool _animstate, 	bool _movespeed, const char* _triggername)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_jumpingstate(_jumpingstate);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_triggername(_triggername);
}

void PROTOFUNC::MakeScPlayerState(SC_PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, bool _jumpingstate, bool _animstate, bool _movespeed, std::string _strTriggerName)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_jumpingstate(_jumpingstate);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_triggername(_strTriggerName);
}

void PROTOFUNC::MakeScMoveState(SC_MOVESTATE* _pOut, LLONG _id, VECTOR3* _pMovePos)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_allocated_movepos(_pMovePos);
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

void PROTOFUNC::MakeCsMove(CS_MOVE* _pOut, LLONG _id, VECTOR3* _pMovePos)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_allocated_movepos(_pMovePos);
}

void PROTOFUNC::MakeCsPlayerState(CS_PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, 
	bool _jumpingstate, bool _animstate, bool _movespeed, const char* _triggername)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_jumpingstate(_jumpingstate);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_triggername(_triggername);
}

void PROTOFUNC::MakeCsPlayerState(CS_PLAYERSTATE* _pOut, LLONG _id, bool _ifattack, 
	bool _jumpingstate, bool _animstate, bool _movespeed, std::string _strTriggerName)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_ifattack(_ifattack);
	_pOut->set_jumpingstate(_jumpingstate);
	_pOut->set_animstate(_animstate);
	_pOut->set_movespeed(_movespeed);
	_pOut->set_triggername(_strTriggerName);
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
