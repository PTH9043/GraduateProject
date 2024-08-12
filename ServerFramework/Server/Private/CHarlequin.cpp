#include "ServerDefines.h"
#include "CHarlequin.h"
#include "ATransform.h"
#include "CHarlequinAnimController.h"
#include "ACollider.h"
#include "ASession.h"
#include "AAnimator.h"
#include "AAnimation.h"
#include "ATransform.h"
#include "CShurikenThrowing.h"

namespace Server {

	CHarlequin::CHarlequin(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) :
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_HARLEQUINN);
		UpdateFindRange(80, 150);
		SetMoveSpeed(5);
		SetAttackRange(9.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1500 });

		SESSIONID id = 0;
		for (auto& iter : m_ShurikenContainer)
		{
			iter = Create<CShurikenThrowing>(GetCoreInstance(), id++);
		}
	}

	_bool CHarlequin::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f) * _float4x4::CreateRotationY(DirectX::XMConvertToRadians(180));
		SetAnimController(Create<CHarlequinAnimController>(GetCoreInstance(), ThisShared<CHarlequin>(),
			"..\\..\\Resource\\Anim\\Harlequin\\", "Harlequin1_FBX.bin", Matrix));

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {3.f, 10.f, 3.f}, {0.f, 10.f, 0.f} });

		__super::Start(_ReceiveDatas);

		SHPTR<ATransform> spTransform = GetTransform();
		_float4x4 mWorldMatrix = spTransform->GetWorldMatrix();

		//for (auto& iter : m_ShurikenContainer)
		//{
		//	iter->Start({ &mWorldMatrix });
		//}
		return true;
	}

	void CHarlequin::Tick(const _double& _dTimeDelta)
	{
		SHPTR<ASession> spSession = GetTargetSession();
		TickUpdateBehavior(_dTimeDelta, spSession);

		SHPTR<AAnimator> spAnimator = GetAnimController()->GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();
		_string strCurAnimName = spAnimation->GetAnimName();

		DEFINE_STATIC_CONSTCHAR(JUMP_ANIMNAME, "Jump Forward");
		DEFINE_STATIC_CONSTCHAR(ATTACK4_ANIMNAME, "Attack 4");
		DEFINE_STATIC_CONSTCHAR(ATTACK3_ANIMNAME, "Attack 3");

		if (strCurAnimName != JUMP_ANIMNAME && strCurAnimName != ATTACK3_ANIMNAME &&
			strCurAnimName != ATTACK4_ANIMNAME)
		{
			RestrictPositionToNavi();
		}

		UpdateColliderData();
		ResetTargetPlayer();
	}

	void CHarlequin::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		FindPlayer(_spSession);

		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();

		_bool isCurrentFindPlayer = IsCurrentFindPlayer();
		_bool isDamaged = IsDamaged();

		if (true == IsDeadStateEnable())
		{
			// 영구적 비활성화
			ActivePermanentDisable();
		}
		else
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

			if (true == isCurrentFindPlayer)
			{
				for (_int i = 0; i < THROWING_NUM; ++i)
				{
					SHPTR<CShurikenThrowing> spShuriken = GetShurikenThrowing(i);
					SHPTR<ATransform> spTransform = spShuriken->GetTransform();

		//			static SC_HARLEQUINTHROWING scHarlequinThrowing;
			//		PROTOFUNC::MakeScHarlequinThrowing(&scHarlequinThrowing, i, );
				}
			}
		}
	}

	void CHarlequin::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CHarlequin::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		__super::Collision(_pGameObject, _dTimeDelta);
	}

	SHPTR<CShurikenThrowing> CHarlequin::GetShurikenThrowing(_int _iIndex)
	{
		RETURN_CHECK(_iIndex >= THROWING_NUM, nullptr);
		return m_ShurikenContainer[_iIndex];
	}

	void CHarlequin::Free()
	{
	}

}