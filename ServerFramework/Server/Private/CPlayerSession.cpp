#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"
#include "ANavigation.h"
#include "AMonster.h"
#include "AAnimController.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 100 },
		m_iWComboStack{ 0 }, m_iSComboStack{ 0 }
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

		SHPTR<ANavigation> spNavigation = GetCoreInstance()->GetNavigation();

		SHPTR<ACell> spCell = spNavigation->FindCell(m_iStartCellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
		return true;
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

	_bool CPlayerSession::ProcessPacket(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		_int SessionID = GetSessionID();
		::memset(GetCopyBufferPointer(), 0, MAX_BUFFER_LENGTH);

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				LoginState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_RESOURCE_RECEIVE_SUCCESS:
			{

			}
				break;
			case TAG_CS::TAG_CS_MOVE:
			{
				MoveState(spCoreInstance, SessionID, _pPacket, _PacketHead);
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PlayerState(spCoreInstance, SessionID, _pPacket, _PacketHead);
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
		}
		// Monster의 패킷들을 미리 보내버린다. 
		{
			SC_MONSTERRESOURCEDATA scMonsterResourceData;
			VECTOR3 vPos;
			VECTOR3 vRotate;
			VECTOR3 vScale;
			const MOBOBJCONTAINER& GameObjectContainer = _spCoreInstance->GetMobObjContainer();
			for (auto& iter : GameObjectContainer)
			{
				if (true == iter.second->IsActive())
					continue;

				if (true == iter.second->IsPermanentDisable())
					continue;

				SHPTR<AAnimController> spAnimController = iter.second->GetAnimController();
				Vector3 vPosVector = iter.second->GetTransform()->GetPos();
				Vector3 vRotateVector = iter.second->GetTransform()->GetRotationValue();
				Vector3 vScaleVector = iter.second->GetTransform()->GetScale();
				PROTOFUNC::MakeVector3(&vPos, vPosVector.x, vPosVector.y, vPosVector.z);
				PROTOFUNC::MakeVector3(&vRotate, vRotateVector.x, vRotateVector.y, vRotateVector.z);
				PROTOFUNC::MakeVector3(&vScale, vScaleVector.x, vScaleVector.y, vScaleVector.z);
				PROTOFUNC::MakeScMonsterResourceData(&scMonsterResourceData, iter.first, vPos, vRotate,
					vScale, spAnimController->GetCurAnimIndex(), iter.second->GetMonsterType());

				CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scMonsterResourceData, TAG_SC::TAG_SC_MONSTERRESOURCEDATA);
				SendData(GetCopyBufferPointer(), GetPacketHead());
			}
			// 모든 자원들을 보내는데 성공했으면 
			{
				SC_START_INFORMATION_SUCCESS		scStartInfomationSuccess;
				PROTOFUNC::MakeScStartInformationSucess(&scStartInfomationSuccess, GetSessionID(), (_int)GameObjectContainer.size());
				CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), scStartInfomationSuccess, TAG_SC::TAG_SC_START_INFORMATION_SUCCESS);
				SendData(GetCopyBufferPointer(), GetPacketHead());
			}
		}
	}

	void CPlayerSession::MoveState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		SHPTR<ANavigation> spNavigation = _spCoreInstance->GetNavigation();

		VECTOR3			vSendPosition;
		VECTOR3			vSendRotate;
		Vector3				vPosition;
		Vector3				vRotate;

		CHARMOVE csMove;
		csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		{
			vPosition = Vector3(csMove.movex(), csMove.movey(), csMove.movez());
			vRotate = Vector3(csMove.rotatex(), csMove.rotatey(), csMove.rotatez());
		}
		_bool IsMove = false;
		SHPTR<ACell> spCurrentCell{ nullptr };
		// 네비게이션을 통해 이동할 수 없는 위치를 표시한다. 
		{
			IsMove = spNavigation->IsMove(GetCurOnCellIndex(), vPosition, REF_OUT spCurrentCell);
			if (true == IsMove)
			{
				GetTransform()->SetPos(vPosition);
				SetCurOnCellIndex(spCurrentCell->GetIndex());
			}
			else
			{
				vPosition = GetTransform()->GetPos();
			}
			PROTOFUNC::MakeVector3(OUT & vSendPosition, vPosition.x, vPosition.y, vPosition.z);
			PROTOFUNC::MakeVector3(OUT & vSendRotate, vRotate.x, vRotate.y, vRotate.z);
		}
		// 몬스터 활성화 
		{
			const MOBOBJCONTAINER& MobObjectContainer = _spCoreInstance->GetMobObjContainer();
			for (auto& iter : MobObjectContainer)
			{
				// 영구적 비활성화
				if (true == iter.second->IsPermanentDisable())
					continue;
				// 거리를 받아옴
				_float fDistanace =  iter.second->GetTransform()->ComputeDistance(vPosition);
				if (fDistanace <= SEE_RANGE)
				{
					iter.second->InsertMobJobTimer(GetSessionID());
				}
				else
				{
					iter.second->SetActiveStrong(false);
				}
			}
		}
		// 만약 움직일 수 없으면
		if (false == IsMove)
		{
			// 이전 위치로 이동할 수 있도록 한다. 
			SELFPLAYERMOVE selfPlayerMove;
			PROTOFUNC::MakeSelfPlayerMove(OUT & selfPlayerMove, GetSessionID(), vSendPosition);
			PROTOFUNC::MakeCharMove(OUT & csMove, _SessionID, vSendPosition, vSendRotate);
			SendProtoData(REF_OUT GetCopyBuffer(), selfPlayerMove, TAG_SC::TAG_SC_SELFPLAYERMOVE);
		}
		//  다른 모든 클라이언트에 메시지 보낸다. 
		{
			CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), csMove, TAG_SC::TAG_SC_CHARMOVE);
			_spCoreInstance->BroadCastMessageExcludingSession(_SessionID, GetCopyBufferPointer(), GetPacketHead());
		}
	}

	void CPlayerSession::PlayerState(SHPTR<ACoreInstance> _spCoreInstance, SESSIONID _SessionID, _char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		PLAYERSTATE PlayerState;
		PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);
		CombineProto(REF_OUT GetCopyBuffer(), REF_OUT GetPacketHead(), PlayerState, TAG_SC::TAG_SC_PLAYERSTATE);
		_spCoreInstance->BroadCastMessageExcludingSession(PlayerState.id(), GetCopyBufferPointer(), GetPacketHead());
	}

	void CPlayerSession::Free()
	{
	}
}


/*
				// Login Packet을 조합하고 메시지를 보낸다.
				{
					const MOBOBJCONTAINER& GameObjectContainer = spCoreInstance->GetMobObjContainer();
					SC_VIEWINRANGE scViewInRange;
					// GameObject
					SET<AMonster*> GameObjectList;
					{
						// 시야처리
						for (auto& iter : GameObjectContainer)
						{
							GameObjectList.insert(iter.second.get());
						}
						SC_VIEWINRANGE scViewRange;
						PROTOALLOC(VECTOR3, position);
						// 해당 녀석들이 있다고 보낸다.
						for (auto& iter : GameObjectList)
						{
							if (Login.id() == iter->GetSessionID())
								continue;

							_int GameObjectID = GetGameObjectType();

							SHPTR<ATransform> spTransform = iter->GetTransform();
							Vector3 vPosition = spTransform->GetPos();
							PROTOFUNC::MakeVector3(OUT  position, vPosition.x, vPosition.y, vPosition.z);
							PROTOFUNC::MakeScViewInRange(OUT &scViewRange, iter->GetSessionID(), IN position, iter->GetCellIndex(), GameObjectID);
							CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scViewInRange, TAG_SC::TAG_SC_VIEWINRANGE);
							spCoreInstance->DirectSendMessage(iter->GetSessionID(), &m_CopyBuffer[0], m_CopyPacketHead);
						}
					}
				}
*/