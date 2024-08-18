#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"
#include "ANavigation.h"
#include "AMonster.h"
#include "AAnimController.h"
#include "ACollider.h"
#include "AStaticObject.h"
#include "CPlayerAnimController.h"
#include "AGameTimer.h"

namespace Server {

	Vector3						CPlayerSession::s_vSavePosition;
	 _int								CPlayerSession::s_iActiveSavePoint;
	 _int								CPlayerSession::s_iCamCellIndex;
	 _int								CPlayerSession::s_iCoreEnableCnt;

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)),
		m_iStartCellIndex{ 0 }, m_spGameTimer{ Create<AGameTimer>() }
	{
	
	}

	_bool CPlayerSession::Start(const VOIDDATAS& _ReceiveDatas)
	{
		__super::Start(_ReceiveDatas);

		SC_CONNECTSUCCESS scConnectSuccess;
		{
			PROTOFUNC::MakeScConnectSuccess(OUT & scConnectSuccess, GetSessionID(), m_iStartCellIndex, TAG_MAINPLAYER);
		}
		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scConnectSuccess, TAG_SC::TAG_SC_CONNECTSUCCESS);

		SendData(GetCopyBufferPointer(), GetPacketHead());
		SetGameObjectType(TAG_CHAR::TAG_MAINPLAYER);
		SetMoveSpeed(10.f);
		SetRunSpeed(30.f);
		SetCurOnCellIndex(m_iStartCellIndex);
		SetCharStatus(CHARSTATUS{ 100, 0, 5000 });

		SHPTR<ANavigation> spNavigation = GetNavigation();

		SHPTR<ACell> spCell = spNavigation->FindCell(m_iStartCellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {4.f, 10.f, 4.f}, {0.f, 10.f, 0.f} });
		InsertColliderContainer(COLLIDERTYPE::COLLIDER_FORATTACK, ACollider::TYPE_OBB,
			COLLIDERDESC{ {4.f, 10.f, 4.f}, {0.f, 10.f, 0.f} });

		SetActive(true);
		s_vSavePosition = GetTransform()->GetPos();
		s_iCamCellIndex = 1;
		return true;
	}

	void CPlayerSession::Tick(const _double& _dTimeDelta)
	{
		__super::Tick(_dTimeDelta);
	}

	void CPlayerSession::RecvData()
	{
		__super::RecvData();
	}

	_bool CPlayerSession::SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(true == IsPermanentDisable() , false);

		CombineSendBuffer( _pPacket, _PacketHead);
		_bool result{ true };

		TCPSOCKET& Socket = GetTcpSocket(REF_RETURN);

		Socket.async_write_some(Asio::buffer(GetSendBuff(), _PacketHead.PacketSize + PACKETHEAD_SIZE),
			[this](const boost::system::error_code& _error, std::size_t _Size) {
			SESSIONID SessionID = GetSessionID();
			Core::SHPTR<Core::ACoreInstance> spCoreInstance{ GetCoreInstance() };
				// Packet
				if (_error)
				{
					if (nullptr != spCoreInstance)
						spCoreInstance->LeaveService(SessionID);
				}
			});
		return result;
	}

	void CPlayerSession::Disconnect()
	{
		__super::Disconnect();
	}

	void CPlayerSession::ConnectTcpSocket(){}

	void CPlayerSession::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{

	}

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		_int SessionID = GetSessionID();

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				LoginState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PlayerState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS_PLAYERDAMAGED:
			{
				PlayerCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_MONSTERCOLIISION:
			{
				MonsterCollisionState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PRESSKEY:
			{
				PressKeyState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_SAVEPOINTENABLE:
			{
				EnableSavePoint(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_COREENABLE:
			{
				EnableCore(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
		}
		return true;
	}

	void CPlayerSession::LoginState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		// 현재 받아온 패킷을 재해석 한다. 
		CS_LOGIN Login;
		Login.ParseFromArray(_pPacket, _PacketHead.PacketSize);
#ifdef USE_DEBUG
		std::cout << "Session ID Login Success Login [" << Login.id() << "]\nCurSession [" << _SessionID << "]\n";
#endif
		// 로그인 된 플레이어의 패킷을 보낸다. 
		{
			// Make Sc Login 
			SC_OTHERCLIENTLOGIN scOtherLogin;
			// 다른 클라이언트들에게 해당 플레이어가 접속했음을 알림
			PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, _SessionID, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());

			for (auto& iter : _spCoreInstance->GetSessionContainer())
			{
				if (iter.first == _SessionID)
					continue;

				if (true == iter.second->IsPermanentDisable())
					continue;

				PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, iter.first, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
				CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
				SendData(GetCopyBufferPointer(), GetPacketHead());
			}
			const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
			for (auto& iter : MobObjectContainer)
			{
				if (true == iter.second->IsPermanentDisable())
				{
					iter.second->RunningPermanentDisableSituation();
				}
			}
		}
	}

	void CPlayerSession::PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		m_spGameTimer->Tick();
		Tick(m_spGameTimer->GetDeltaTime());

		SHPTR<ANavigation> spNavigation = GetNavigation();
		SHPTR<ATransform> spTransform = GetTransform();
		SHPTR<ACell> spCurCell = nullptr;
		Vector3				vPosition;

		CHARSTATE csMove;
		csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		{
			vPosition = Vector3(csMove.posx(), csMove.posy(), csMove.posz());
			spTransform->SetPos(vPosition);
		}
		if (true == spNavigation->IsMove(vPosition, spCurCell))
		{
			if (spCurCell->GetIndex() == 1141)
			{
				if (3 == s_iCoreEnableCnt)
				{
					SC_ENDING scEnding;
					PROTOFUNC::MakeScEnding(&scEnding, GetSessionID());
					CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scEnding, TAG_SC::TAG_SC_ENDING);
					_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				}
			}
		}
		if (true == IsDead())
		{
			if (GetKeyState() == KEYBOARD_G)
			{
				VECTOR3 vPos;
				PROTOFUNC::MakeVector3(&vPos, s_vSavePosition.x, s_vSavePosition.y, s_vSavePosition.z);

				SC_PLAYERGETUP scPlayerGetUp;
				PROTOFUNC::MakeScPlayerGetUp(&scPlayerGetUp, GetSessionID(), 5000.f, vPos, s_iCamCellIndex);
				CombineProto<SC_PLAYERGETUP>(GetCopyBuffer(), GetPacketHead(), scPlayerGetUp, TAG_SC_PLAYERGETUP);
				SendData(GetCopyBufferPointer(), GetPacketHead());
				SetCharStatus({ 100, 0, 5000 });
				DisableDeadState();
			}
		}
		{
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), csMove, TAG_SC::TAG_SC_PLAYERSTATE);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());
		}

		SetCurrentAnimState(csMove.state());

		if (false == IsFallDownState())
		{
			if (PLAYER_ANIM_FALLDOWN == csMove.state())
			{
				EnableFallDownState();
			}
		}
		
		{
			const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
			SHPTR<ASession> spSession = ThisShared<ASession>();
			for (auto& iter : MobObjectContainer)
			{
				// 영구적 비활성화
				if (true == iter.second->IsPermanentDisable())
					continue;

				// 거리를 받아옴
				if (true == iter.second->IsCanSee(spTransform))
				{
					iter.second->State(spSession);
				}
			}

			const STATICOBJCONTAINER& StaticObjContainer = _spCoreInstance->GetStaticObjContainer();
			for (auto& iter : StaticObjContainer)
			{
				if (true == iter.second->IsPermanentDisable())
					continue;

				// 거리를 받아옴
				if (true == iter.second->IsCanSee(spTransform))
				{
					iter.second->State(spSession);
				}
			}
		}
	}

	void CPlayerSession::PlayerCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		CS_DAMAGEDTOMONSTER CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<ASession> spSession = _spCoreInstance->FindSession(CollisionData.id());
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.enemyid());
		if (nullptr == spMonster)
		{
			SC_DAMAGED Damage;
			PROTOFUNC::MakeScDamaged(&Damage, GetSessionID(), spSession->Damaged(CollisionData.damage()));
			CombineProto<SC_DAMAGED>(GetCopyBuffer(), GetPacketHead(), Damage, TAG_SC_DAMAGED);
			_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			Damage.Clear();
			// Dead
			if (true == spSession->IsDead())
			{
				_spCoreInstance->CheckAllPlayerDie();
			}
		}
		else
		{
			if (true == spMonster->IsDamagedToEnemyTimerPass() || true == spSession->IsDead())
			{
				SC_DAMAGED Damage;
				PROTOFUNC::MakeScDamaged(&Damage, GetSessionID(), spSession->Damaged(CollisionData.damage()));
				CombineProto<SC_DAMAGED>(GetCopyBuffer(), GetPacketHead(), Damage, TAG_SC_DAMAGED);
				_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
				Damage.Clear();
				// Dead
				if (true == spSession->IsDead())
				{
					_spCoreInstance->CheckAllPlayerDie();
				}
				spMonster->ResetDamagedToEnemyTimer();
			}
		}
		CollisionData.Clear();
	}

	void CPlayerSession::MonsterCollisionState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		CS_DAMAGED CollisionData;
		CollisionData.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SHPTR<AMonster> spMonster = _spCoreInstance->FindMobObject(CollisionData.id());
		
		if (true == IsDamagedToEnemyTimerPass() || spMonster->IsDead())
		{
			SC_DAMAGED Damage;
			PROTOFUNC::MakeScDamaged(&Damage, spMonster->GetSessionID(), spMonster->Damaged(CollisionData.damage()));
			CombineProto<SC_DAMAGED>(GetCopyBuffer(), GetPacketHead(), Damage, TAG_SC_DAMAGED);
			_spCoreInstance->BroadCastMessage(GetCopyBufferPointer(), GetPacketHead());
			ResetDamagedToEnemyTimer();
			Damage.Clear();
		}
		CollisionData.Clear();
	}

	void CPlayerSession::PressKeyState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		CS_PRESSKEY scPressKey;
		scPressKey.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		SetKeyState(scPressKey.key());
	}

	void CPlayerSession::EnableSavePoint(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		CS_SAVEPOINTENABLE csSavePointEnable;
		csSavePointEnable.ParseFromArray(_pPacket, _PacketHead.PacketSize);

		s_vSavePosition = Vector3(csSavePointEnable.posx(), csSavePointEnable.posy(), csSavePointEnable.posz());
		++s_iActiveSavePoint;
		s_iCamCellIndex = csSavePointEnable.camcellindex();

		for (auto& iter : _spCoreInstance->GetSessionContainer())
		{
			SC_SAVEPOINTENABLE scSavePointEnable;
			PROTOFUNC::MakeScSavePointEnable(&scSavePointEnable, iter.first, s_iActiveSavePoint);
			CombineProto<SC_SAVEPOINTENABLE>(GetCopyBuffer(), GetPacketHead(), scSavePointEnable, TAG_SC_SAVEPOINTENABLE);
			iter.second->SendData(GetCopyBufferPointer(), GetPacketHead());
		}

	//	std::cout << s_iCamCellIndex << "\n";
	}

	void CPlayerSession::EnableCore(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		++s_iCoreEnableCnt;

	//	std::cout << s_iCoreEnableCnt << "\n";
	}

	void CPlayerSession::Free()
	{
	}
}
