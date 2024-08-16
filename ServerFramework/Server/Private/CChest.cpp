#include "ServerDefines.h"
#include "CChest.h"
#include "ATransform.h"
#include "AAnimController.h"
#include "CChestAnimController.h"
#include "ACoreInstance.h"
#include "ASession.h"
#include "AAnimator.h"

namespace Server {
	CChest::CChest(OBJCON_CONSTRUCTOR, SESSIONID _ID, SHPTR<AJobTimer> _spMonsterJobTimer) : 
		CServerMonster(OBJCON_CONDATA, _ID, _spMonsterJobTimer)
	{
		SetMonsterType(TAG_CHAR::TAG_CHEST);
		UpdateFindRange(10.f, 20.f);
		SetMoveSpeed(5);
		SetAttackRange(3.f);
		SetCharStatus(CHARSTATUS{ 1, 0, 1000 });
	}

	_bool CChest::Start(const VOIDDATAS& _ReceiveDatas)
	{
		_float4x4 Matrix = _float4x4::CreateScale(0.1f);
		SetAnimController(Create<CChestAnimController>(GetCoreInstance(), ThisShared<CChest>(),
			"..\\..\\Resource\\Anim\\Chest\\", "Chest 1_FBX.bin", Matrix));
		return __super::Start(_ReceiveDatas);
	}

	void CChest::Tick(const _double& _dTimeDelta)
	{
		static constexpr _double ItemChestOpeningSpeed = 2;
		static constexpr _double ItemChestTimeArcOpenEnd = 3;

		if (GetElapsedTime() < ItemChestTimeArcOpenEnd)
		{
			SHPTR<ATransform> spTransform = GetTransform();
			SHPTR<AAnimController> spAnimController = GetAnimController();
			SHPTR<AAnimator> spAnimator = spAnimController->GetAnimator();

			SetElapsedTime(GetElapsedTime() + _dTimeDelta * ItemChestOpeningSpeed);
			spAnimator->TickAnimToTimeAccChangeTransform(spTransform, _dTimeDelta, GetElapsedTime());
		}

		UpdateColliderData();
		DeadStateEnable(_dTimeDelta);
	}

	void CChest::State(SHPTR<ASession> _spSession, _int _MonsterState)
	{
		if (true == IsDead())
		{
			ActivePermanentDisable();
		}
		else
		{
			FindPlayer(_spSession);

			if (true == IsCurrentFindPlayer())
			{
				SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
				_int KeyState = _spSession->GetKeyState();
				_int enable = KeyState == KEYBOARD_F ? 1 : 0;
				SC_STATICOBJFIND scMonsterFind;
				PROTOFUNC::MakeScStaticObjFind(&scMonsterFind, GetSessionID(), enable);
				CombineProto<SC_STATICOBJFIND>(GetCopyBuffer(), GetPacketHead(), scMonsterFind, TAG_SC_STATICOBJFIND);
				spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				if (1 == enable)
				{
					SHPTR<ASession> spSession = _spSession;
					spSession->HealHp(210.f);
					ActivePermanentDisable();
				}
			}
		}
	}

	void CChest::ProcessPacket(_int _type, void* _pData)
	{
		__super::ProcessPacket(_type, _pData);
	}

	void CChest::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	//	__super::Collision(_pGameObject, _dTimeDelta);
	}

	void CChest::LastBehavior()
	{
	}

	void CChest::Free()
	{
	}

}