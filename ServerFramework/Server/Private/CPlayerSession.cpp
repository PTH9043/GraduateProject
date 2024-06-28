#include "ServerDefines.h"
#include "CPlayerSession.h"
#include "ACoreInstance.h"
#include "AServerService.h"
#include "ATransform.h"
#include "ACell.h"

namespace Server {

	CPlayerSession::CPlayerSession(SESSION_CONSTRUCTOR)
		: Core::ASession(SESSION_CONDATA(Core::SESSIONTYPE::PLAYER)), m_iStartCellIndex{ 741 },
		m_iWComboStack {0},	m_iSComboStack{0}
	{
		::memset(&m_CopyBuffer[0], 0, sizeof(BUFFER));
		::memset(&m_CopyPacketHead, 0, sizeof(PACKETHEAD));
	}

	_bool CPlayerSession::Start()
	{
		__super::Start();

		SC_CONNECTSUCCESS scConnectSuccess;
		{
			PROTOFUNC::MakeScConnectSuccess(OUT & scConnectSuccess, GetSessionID(), m_iStartCellIndex, TAG_MAINPLAYER);
		}
		CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scConnectSuccess, TAG_SC::TAG_SC_CONNECTSUCCESS);

		SendData(&m_CopyBuffer[0], m_CopyPacketHead);
		SetGameObjectType(TAG_CHAR::TAG_MAINPLAYER);
		SetMoveSpeed(10.f);
		SetRunSpeed(30.f);
		SetCurOnCellIndex(m_iStartCellIndex);
		SHPTR<ACell> spCell = GetCoreInstance()->FindCell(m_iStartCellIndex);
		GetTransform()->SetPos(spCell->GetCenterPos());
		return true;
	}

	void CPlayerSession::RecvData()
	{
		__super::RecvData();
	}

	_bool CPlayerSession::SendData(_char* _pPacket, const Core::PACKETHEAD& _PacketHead)
	{
		RETURN_CHECK(false == IsConnected() , false);

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
				else
				{
					if (false == IsConnected())
					{
						if (nullptr != spCoreInstance)
						{
							spCoreInstance->LeaveService(SessionID);
						}
					}
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

		switch (_PacketHead.PacketType)
		{
			case TAG_CS::TAG_CS_LOGIN:
			{
				// 현재 받아온 패킷을 재해석 한다. 
				CS_LOGIN Login;
				Login.ParseFromArray(_pPacket, _PacketHead.PacketSize);
				std::cout << "Session ID Login Success [" << Login.id() << "]\n";
				{
					// Make Sc Login 
					SC_OTHERCLIENTLOGIN scOtherLogin;
					// 다른 클라이언트들에게 해당 플레이어가 접속했음을 알림
					PROTOFUNC::MakeScOtherClientLogin(OUT  &scOtherLogin, GetSessionID(), m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
					CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
					spCoreInstance->BroadCastMessageExcludingSession(GetSessionID(), &m_CopyBuffer[0], m_CopyPacketHead);

					for (auto& iter : spCoreInstance->GetSessionContainer())
					{
						if (iter.first == GetSessionID())
							continue;

						if (nullptr == iter.second)
							continue;

						PROTOFUNC::MakeScOtherClientLogin(OUT & scOtherLogin, iter.first, m_iStartCellIndex, TAG_CHAR::TAG_OTHERPLAYER);
						CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, scOtherLogin, TAG_SC::TAG_SC_OTHERCLIENTLOGIN);
						SendData(&m_CopyBuffer[0], m_CopyPacketHead);
					}
				}
				// Login Packet을 조합하고 메시지를 보낸다. 
				{
					const GAMEOBJECTCONTAINER& GameObjectContainer = spCoreInstance->GetGameObjectContainer();
					SC_VIEWINRANGE scViewInRange;
					// GameObject
					SET<AGameObject*> GameObjectList;
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
			}
			break;
			case TAG_CS::TAG_CS_MOVE:
			{
				CHARMOVE csMove;
				csMove.ParseFromArray(_pPacket, _PacketHead.PacketSize);
				Vector3 vPos = Vector3(csMove.movex(), csMove.movey(), csMove.movez());
				Vector4 vRotate = Vector4(csMove.rotatex(), csMove.rotatey(), csMove.rotatez(), csMove.rotatew());
				SHPTR<ACell> spCurrentCell{ nullptr };
				_bool IsMove = false;
				if (csMove.id() == GetSessionID())
				{
					IsMove = spCoreInstance->IsMove(GetCurOnCellIndex(), vPos, REF_OUT spCurrentCell);
					if (true == IsMove)
					{
						GetTransform()->SetPos(vPos);
						GetTransform()->RotateFix(vRotate);
						SetCurOnCellIndex(spCurrentCell->GetIndex());
					}
					else
					{
						vPos = GetTransform()->GetPos();
					}
				}
				VECTOR3 vPosition;
				VECTOR4 vRotation;
				PROTOFUNC::MakeVector3(OUT & vPosition, vPos.x, vPos.y, vPos.z);
				PROTOFUNC::MakeVector4(OUT & vRotation, vRotate.x, vRotate.y, vRotate.z, vRotate.w);
				if(false == IsMove)
				{
					SELFPLAYERMOVE selfPlayerMove;
					PROTOFUNC::MakeSelfPlayerMove(OUT & selfPlayerMove,  GetSessionID(), vPosition);
					SendProtoData(m_CopyBuffer, selfPlayerMove, TAG_SC::TAG_SC_SELFPLAYERMOVE);
				}
				{
					PROTOFUNC::MakeCharMove(OUT & csMove, GetSessionID(), vPosition, vRotation, csMove.jumpingstate());
					CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, csMove, TAG_SC::TAG_SC_CHARMOVE);
					spCoreInstance->BroadCastMessageExcludingSession(GetSessionID(), &m_CopyBuffer[0], m_CopyPacketHead);
				}
			}
			break;
			case TAG_CS::TAG_CS_PLAYERSTATE:
			{
				PLAYERSTATE PlayerState;
				PlayerState.ParseFromArray(_pPacket, _PacketHead.PacketSize);

				CombineProto(REF_OUT m_CopyBuffer, REF_OUT m_CopyPacketHead, PlayerState, TAG_SC::TAG_SC_PLAYERSTATE);
				m_CopyPacketHead.PacketSize = _PacketHead.PacketSize;
		//		std::cout << PlayerState.triggername() << "\n";
				spCoreInstance->BroadCastMessageExcludingSession(PlayerState.id(), &m_CopyBuffer[0], m_CopyPacketHead);
	//		spCoreInstance->BroadCastMessage(&m_CopyBuffer[0], m_CopyPacketHead);
			}
			break;
		}
		return true;
	}

	void CPlayerSession::Free()
	{
	}
}
