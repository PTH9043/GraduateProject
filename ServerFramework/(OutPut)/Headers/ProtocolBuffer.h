
#include "Protocol.pb.h"
#include "Tag.pb.h"

#define _ENABLE_PROTOBUFF

namespace PROTOFUNC
{

	using LLONG =  long long;

	void MakeCharData(CHARDATA* _pOut, float _power, float _defensive, float _hp);
	void MakeVector3(VECTOR3* _pOut, float _x, float _y, float _z);
	void MakeEqInfo(EQINFO* _pOut,  float _gold, float _increasePower, float _increaseDefensive, float _increaseHp);

	/* =========== SC =============== */
	// Server To Client 

	void MakeScConnectSuccess(SC_CONNECTSUCCESS* _pOut, LLONG _id,  CHARDATA* _pCharData,
		 int _cellIndex, VECTOR3* _pSize, int _type);
	void MakeScOtherClientLogin(SC_OTHERCLIENTLOGIN* _pOut, LLONG _id, int _cellIndex, int _type);
	void MakeScMoveFailed(SC_MOVEFAILED* _pOut, LLONG _id, VECTOR3* _pPrevPos);
	void MakeScOtherMove(SC_OTHERMOVE* _pOut, LLONG _id, VECTOR3* _pMovePos);
	void MakeScViewInRange(SC_VIEWINRANGE* _pOut, LLONG _id, VECTOR3* _pPos, int _cellIndex, int _type);

	/* =========== CS =============== */
	// Client To Server 

	void MakeCsLogin(CS_LOGIN* _pOut, LLONG _id);
	void MakeCsMove(CS_MOVE* _pOut, LLONG _id, VECTOR3* _pMovePos);
	void MakeCsAttack(CS_ATTACK* _pOut, LLONG _id, float _damage, VECTOR3* _pMovePos);
	void MakeCsDisconnect(CS_DISCONNECT* _pOut, LLONG _id);

	class PROTOCOL_MEMORY_LEAK_REMOVER
	{
	public:
		PROTOCOL_MEMORY_LEAK_REMOVER(){}
		~PROTOCOL_MEMORY_LEAK_REMOVER();
	};
}

static PROTOFUNC::PROTOCOL_MEMORY_LEAK_REMOVER MemoryRemover;