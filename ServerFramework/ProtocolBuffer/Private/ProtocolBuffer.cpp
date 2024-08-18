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

void PROTOFUNC::MakeCollisionData(COLLISIONDATA* _pOut, LLONG _id, LLONG _enemyID)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_enemyid(_enemyID);
}

void PROTOFUNC::MakeCharState(CHARSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate,
	int _animstate, int _curAnimIndex, int _triggerOn)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_rotatex(_vRotate.x());
	_pOut->set_rotatey(_vRotate.y());
	_pOut->set_rotatez(_vRotate.z());
	_pOut->set_state(_animstate);
	_pOut->set_animationindex(_curAnimIndex);
	_pOut->set_triggeron(_triggerOn);
}

void PROTOFUNC::MakeCharState(CHARSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_rotatex(_vRotate.x());
	_pOut->set_rotatey(_vRotate.y());
	_pOut->set_rotatez(_vRotate.z());
}

void PROTOFUNC::MakeMobState(MOBSTATE* _pOut, LLONG _id, const VECTOR3& _vPos, const VECTOR3& _vRotate,
	int _animstate, int _curAnimIndex, bool _triggerOn, bool _foundOn, bool _damageOn, double _animtime)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_rotatex(_vRotate.x());
	_pOut->set_rotatey(_vRotate.y());
	_pOut->set_rotatez(_vRotate.z());
	_pOut->set_state(_animstate);
	_pOut->set_animationindex(_curAnimIndex);
	_pOut->set_triggeron(_triggerOn);
	_pOut->set_foundon(_foundOn);
	_pOut->set_damageon(_damageOn);
	_pOut->set_animtime(_animtime);
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

void PROTOFUNC::MakeScMonsterFind(SC_MONSTERFIND* _pOut, LLONG _id, int _targetID)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_targetid(_targetID);
}

void PROTOFUNC::MakeScDamaged(SC_DAMAGED* _pOut, LLONG _id, float _hp)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_hp(_hp);
}

void PROTOFUNC::MakeScSeePlayerMove(SC_SEEPLAYERMOVE* _pOut, LLONG _id, const VECTOR3& _vPos)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
}

void PROTOFUNC::MakeScHarlequinThrowing(SC_HARLEQUINTHROWING* _pOut, LLONG _id, const VECTOR3& _vPos,
	int _throwenable, int _index)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_throwon(_throwenable);
	_pOut->set_index(_index);
}

void PROTOFUNC::MakeScStaticObjFind(SC_STATICOBJFIND* _pOut, LLONG _id, int _enable)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_enable(_enable);
}

void PROTOFUNC::MakeScAnubisState(SC_ANUBISSTATE* _pOut, LLONG _id, int _shieldOn, float _ShieldTimer)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_shieldon(_shieldOn);
	_pOut->set_shieldtimer(_ShieldTimer);
}

void PROTOFUNC::MakeScSavePointEnable(SC_SAVEPOINTENABLE* _pOut, LLONG _id, int _count)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_count(_count);
}

void PROTOFUNC::MakeScPlayerGetUp(SC_PLAYERGETUP* _pOut, LLONG _id, float _hp, const VECTOR3& _vPos, int _camCellIndex, int _cellIndex)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_hp(_hp);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_camcellindex(_camCellIndex);
	_pOut->set_cellindex(_cellIndex);
}

void PROTOFUNC::MakeScEnding(SC_ENDING* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
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

void PROTOFUNC::MakeCsHeal(CS_HEAL* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
}

void PROTOFUNC::MakeCsPlayerOrder(CS_PLAYERORDER* _pOut, LLONG _id, int _playerOrder)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_order(_playerOrder);
}

void PROTOFUNC::MakeCsDamaged(CS_DAMAGED* _pOut, LLONG _id, float _Damaged)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_damage(_Damaged);
}

void PROTOFUNC::MakeCsPressKey(CS_PRESSKEY* _pOut, LLONG _id, int _key)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_key(_key);
}

void PROTOFUNC::MakeCsDamagedToMonster(CS_DAMAGEDTOMONSTER* _pOut, LLONG _id, float _damage, LLONG _enemyID)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_damage(_damage);
	_pOut->set_enemyid(_enemyID);
}

void PROTOFUNC::MakeCsSavePointEnable(CS_SAVEPOINTENABLE* _pOut, LLONG _id, const VECTOR3& _vPos, int _camCellIndex, int _cellIndex)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
	_pOut->set_posx(_vPos.x());
	_pOut->set_posy(_vPos.y());
	_pOut->set_posz(_vPos.z());
	_pOut->set_camcellindex(_camCellIndex);
	_pOut->set_cellindex(_cellIndex);
}

void PROTOFUNC::MakeCsCoreEnable(CS_CORENABLE* _pOut, LLONG _id)
{
	assert(nullptr != _pOut);
	_pOut->set_id(_id);
}