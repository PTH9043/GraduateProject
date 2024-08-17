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
		SetAttackRange(15.f);
		SetCharStatus(CHARSTATUS{ 100, 0, 1 });
	}
	_bool CMummy::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CMummyAnimController>(GetCoreInstance(), ThisShared<CMummy>(),
			"..\\..\\Resource\\Anim\\Mummy\\", "Mummy_DEMO_1_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {1.f, 8.f, 1.f}, {0.f, 10.f, 0.f} });

		__super::Start(_ReceiveDatas);

		if (MUMMY_LAYING == m_eMummyType)
		{
			GetAnimController()->SetAnimation("staticLaying");
		}
		else
		{
			GetAnimController()->SetAnimation("staticStanding");
		}

		SetFoundPlayerFirstTime(false);
		UpdateSelfStateToPlayerDistance(false, false, false);
		return true;
	}

	void CMummy::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);
		__super::Tick(_dTimeDelta);
	}

	void CMummy::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		_bool isFirstFindPlayer = IsFoundPlayerFirstTime();
		_bool isCurrentFindPlayer = IsCurrentFindPlayer();
		_bool isDamaged = IsDamaged();

		if (true == IsDeadStateEnable())
		{
			// 영구적 비활성화
			ActivePermanentDisable();
		}
		else
		{
			if (true == isFirstFindPlayer)
			{
				SHPTR<ATransform> spTransform = GetTransform();
				SHPTR<AAnimController> spAnimController = GetAnimController();
				SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();
				SHPTR<AAnimation> spCurAnimation = spAnimator->GetCurAnimation();

				Vector3 vPos = spTransform->GetPos();
				Vector3 vRotate = spTransform->GetRotationValue();

				VECTOR3 vSendPos, vSendRotate;
				{
					PROTOFUNC::MakeVector3(&vSendPos, vPos.x, vPos.y, vPos.z);
					PROTOFUNC::MakeVector3(&vSendRotate, vRotate.x, vRotate.y, vRotate.z);
				}

				_int AnimState = spAnimController->GetAnimState();
				_double dTimeAcc = spCurAnimation->GetTimeAcc();
				_int AnimIndex = spAnimator->GetCurAnimIndex();

				MOBSTATE monsterState;
				PROTOFUNC::MakeMobState(&monsterState, GetSessionID(), vSendPos, vSendRotate,
					AnimState, AnimIndex, false, isCurrentFindPlayer, isDamaged, dTimeAcc);
				CombineProto<MOBSTATE>(GetCopyBuffer(), GetPacketHead(), monsterState, TAG_SC_MONSTERSTATE);
				_spSession->SendData(GetCopyBufferPointer(), GetPacketHead());
			}
		}
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