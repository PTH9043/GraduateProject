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

void PROTOFUNC::MakeSelfPlayerMove(SELFPLAYERMOVE* _pOut, LLONG _id, const VECTOR3& _move)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_movex(_move.x());
	_pOut->set_movey(_move.y());
	_pOut->set_movez(_move.z());
}

void PROTOFUNC::MakeCharMove(CHARMOVE* _pOut, LLONG _id, const VECTOR3& _move, const VECTOR3& _rotate)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_movex(_move.x());
	_pOut->set_movey(_move.y());
	_pOut->set_movez(_move.z());
	// rotate
	_pOut->set_rotatex(_rotate.x());
	_pOut->set_rotatey(_rotate.y());
	_pOut->set_rotatez(_rotate.z());
}

void PROTOFUNC::MakeMonsterState(MONSTERSTATE* _pOut, LLONG _id, double _animDuration, int _curAnimIndex)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_animationtime(_animDuration);
	_pOut->set_animationindex(_curAnimIndex);
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

void PROTOFUNC::MakeScStartInformationSucess(SC_START_INFORMATION_SUCCESS* _pOut, LLONG _id, int _monsterNum)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_monsternum(_monsterNum);
}

void PROTOFUNC::MakeScMonsterResourceData(SC_MONSTERRESOURCEDATA* _pOut, LLONG _id, const VECTOR3& _vPos, 
	const VECTOR3& _vRotate, const VECTOR3& _vScale, int _animIndex, int _type)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_rotatex(_vRotate.x());
	_pOut->set_rotatey(_vRotate.y());
	_pOut->set_rotatez(_vRotate.z());
	_pOut->set_scalex(_vScale.x());
	_pOut->set_scaley(_vScale.y());
	_pOut->set_scalez(_vScale.z());
	_pOut->set_animindex(_animIndex);
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

void PROTOFUNC::MakeScResourceReceiveSuccess(CS_RESOURCE_RECEIVE_SUCCES* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
}
