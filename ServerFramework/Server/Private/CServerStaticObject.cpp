#include "ServerDefines.h"
#include "CServerStaticObject.h"
#include "ATransform.h"
#include "ACollider.h"
#include "ASession.h"
#include "ACoreInstance.h"

namespace Server {

	CServerStaticObject::CServerStaticObject(OBJCON_CONSTRUCTOR, SESSIONID _ID) :
		AStaticObject(OBJCON_CONDATA, _ID), m_StaticObjType{0}, 
		m_isDoneInteractStaticObject{ false }, m_ActiveTimer{4, std::memory_order_seq_cst}
	{
		SetActiveRange(12.f);
	}

	_bool CServerStaticObject::Start(const VOIDDATAS& _ReceiveDatas)
	{
#ifndef CREATED_SERVERMOBDATA
		MOBDATA* pMobData = static_cast<MOBDATA*>(_ReceiveDatas[0]);
		GetTransform()->SetNewWorldMtx(pMobData->mWorldMatrix);
		m_strRoomName = pMobData->strRoomName;
#else
		STATICOBJSERVERDATA* pStaticObjData = static_cast<STATICOBJSERVERDATA*>(_ReceiveDatas[0]);
		GetTransform()->SetNewWorldMtx(pStaticObjData->mWorldMatrix);
		SetSessionID(pStaticObjData->iServerID);
		m_strRoomName = pStaticObjData->strRoomName;
#endif
		UpdateColliderData();
		return __super::Start(_ReceiveDatas);
	}

	void CServerStaticObject::Tick(const _double& _dTimeDelta)
	{
		if (true == m_isDoneInteractStaticObject)
		{
			m_ActiveTimer.fTimer += (_float)(_dTimeDelta);
		}
	}

	void CServerStaticObject::State(SHPTR<ASession> _spSession)
	{
		FindPlayer(_spSession);
		RETURN_CHECK(nullptr == GetInteractiveSession(), ;);
		RETURN_CHECK(false == IsCurrentFindPlayer(), ;);
		_int enable = 0;

		SESSIONID sessionID = _spSession->GetSessionID();
		_int KeyState = _spSession->GetKeyState();
		if (KeyState == KEYBOARD_F)
		{
			SetDoneInteractiveStaticObject(true);
		}
		else
		{
			SetDoneInteractiveStaticObject(false);
			GetActiveTimerRefP().ResetTimer();
		}
		_bool IsPass = GetActiveTimerRefP(REF_RETURN).IsOver();
		enable = (true == IsPass ? 2 : (true == IsDoneInteractStaticObject() ? 1 : 0));

		SC_STATICOBJFIND scStaticObjectFind;
		PROTOFUNC::MakeScStaticObjFind(&scStaticObjectFind, GetSessionID(), enable);
		CombineProto<SC_STATICOBJFIND>(GetCopyBuffer(), GetPacketHead(), scStaticObjectFind,
			TAG_SC_STATICOBJFIND);
		scStaticObjectFind.Clear();

		if (2 == enable)
		{
			SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
			spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			ActivePermanentDisable();
		}
		else
		{
			_spSession->SendData(GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CServerStaticObject::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
		assert(nullptr != _pGameObject);
		SESSIONTYPE EnemySessionType = _pGameObject->GetSessionType();
		RETURN_CHECK(SESSIONTYPE::PLAYER != EnemySessionType, ;);
		RETURN_CHECK(false == IsCurrentFindPlayer(), ;);
	}

	void CServerStaticObject::LastBehavior()
	{

	}

	void CServerStaticObject::Free()
	{
	}


}