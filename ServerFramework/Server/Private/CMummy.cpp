#include "ServerDefines.h"
#include "CMummy.h"
#include "ATransform.h"
#include "CMummyAnimController.h"
#include "CMobLayoutLoader.h"
#include "ANavigation.h"
#include "ACoreInstance.h"
#include "ACell.h"
#include "AAnimator.h"
#include "AAnimController.h"
#include "AAnimation.h"
#include "ASession.h"
#include "ACollider.h"

namespace Server
{
	CMummy::CMummy(OBJCON_CONSTRUCTOR,  SESSIONID _ID, MUMMYTYPE _MummyType, SHPTR<AJobTimer> _spMonsterJobTimer)
		: CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer),
		m_eMummyType{ _MummyType }
	{
		if (MUMMY_LAYING == m_eMummyType)
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_LAYING);
		}
		else
		{
			SetMonsterType(TAG_CHAR::TAG_MUMMY_STANDING);
		}
		UpdateFindRange(50.f, 90.f);
		SetMoveSpeed(5);
		SetAttackRange(10.f);
		SetCharStatus(CHARSTATUS{ 100, 0, 500 });
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {1.f, 8.f, 1.f}, {0.f, 10.f, 0.f} });

		return __super::Start(_ReceiveDatas);
	}

	void CMummy::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		__super::Tick(_dTimeDelta);
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		__super::State(_spSession, _MonsterState);
	}

	void CMummy::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CMummy::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CMummy::Free()
	{
	}

}