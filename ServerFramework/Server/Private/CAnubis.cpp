#include "ServerDefines.h"
#include "CAnubis.h"
#include "ATransform.h"
#include "CAnubisAnimController.h"
#include "ACollider.h"

namespace Server
{

	CAnubis::CAnubis(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_ANUBIS);
		UpdateFindRange(70, 80);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1000 });
	}
	_bool CAnubis::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CAnubisAnimController>(GetCoreInstance(), ThisShared<CAnubis>(),
			"..\\..\\Resource\\Anim\\Anubis\\", "Anubis_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {0.f, 10.f, 0.f}, {1.f, 8.f, 1.f} });

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAGICSPHERE, ACollider::TYPE_SPHERE,
			COLLIDERDESC{ {8.f, 8.f, 8.f}, {0.f, 5.f, 0.f} });

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAGICCIRCLE, ACollider::TYPE_OBB,
			COLLIDERDESC{ {1.f, 1.f, 1.f}, {0.f, 0.5f, 0.f} });

		return  __super::Start(_ReceiveDatas);
	}

	void CAnubis::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
	}

	void CAnubis::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
	//	__super::State(_spSession, _MonsterState);
	}

	void CAnubis::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CAnubis::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CAnubis::Free()
	{
	}

}