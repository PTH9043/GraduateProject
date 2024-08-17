#pragma once

#include "ServerUtility.h"
#include "UProcessedData.h"

BEGIN(Engine)
class UOverExp;
class UActor;

using TOTALBUFFER = ARRAY<_char, MAX_PROCESSBUF_LENGTH>;
using NETWORKINITDATACONTAINER = UNORMAP<_int, NETWORKRECEIVEINITDATA>;

class UActor;
class UNetworkQueryProcessing;
/*
@ Date: 2024-02-03,  Writer: 박태현
@ Explain
- NetworkBaseController로 상속받아서 사용할 Server와 통신 클래스
*/
class UNetworkBaseController abstract : public UBase {
	using NETWORKACTORCONTAINER = UNORMAP<_int, SHPTR<UActor>>;
public:
	UNetworkBaseController();
	DESTRUCTOR(UNetworkBaseController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) PURE;
	virtual void MakeActorsInit(const VECTOR<SHPTR<UBase>>& _actorContainer) PURE;
	virtual void MakeActorsTick() PURE;
	void AddNetworkInitData(_int _NetworkID, const NETWORKRECEIVEINITDATA& _NetworkInitData);
	void InsertNetworkActorInContainer(_int _NetworkID, CSHPTRREF<UActor> _spActor);
	SHPTR<UActor> FindNetworkActor(const _int _NetworkID);
	void SendTcpData(_char* _pData, short _tag, short _size);
	void SendProtoData(const UProcessedData& _ProcessedData);
	void ServerTick();
public: /* get set */
	const _llong GetNetworkOwnerID() const { return m_llNetworkOwnerID; }
	const NETWORKACTORCONTAINER& GetNetworkActorContainer() const { return m_NetworkActorContainer; }
	SOCKET* GetClientSocketPointer() { return &m_ClientTcpSocket; }

	void SetSceneID(const _int _iSceneID) { this->m_iSceneID = _iSceneID; }
	void DisableNetworkTickRunning() { m_isNetworkTickRunning = false; }
protected:
	virtual void NativePacket() PURE;
	void CombineRecvPacket(UOverExp* _pOverExp, size_t _numBytes);
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) PURE;
	/*
	@ Date: 2023-01-05, Writer: 박태현
	@ Explain
	- ProtocolBuffer를 TCP에 연결된 상대에게 보내기 위한 템플릿 함수이다.
	*/
	template<class T>
		requires CheckProtoType<T>
	void SendProtoData(const T& _data, short _tag)
	{
		static BUFFER Buffer;
		_data.SerializePartialToArray((void*)&Buffer[0], static_cast<int>(_data.ByteSizeLong()));
		short size = static_cast<short>(_data.ByteSizeLong());
		SendTcpData(&Buffer[0], _tag, size);
	}

	void InsertProcessedDataInActor(const UProcessedData& _ProcessedData);
protected: /* get set */
	const SOCKET& GetClientTcpSocket() const { return m_ClientTcpSocket; }
	const SOCKET& GetClientUdpSocket() const { return m_ClientUdpSocket; }
	CSHPTRREF<UNetworkAddress> GetNetworkAddress() const { return m_spNetworkAddress; }
	const NETWORKINITDATACONTAINER& GetNetworkInitDataContainer() const { return m_NetworkInitDataContainer; }
	const _int GetSceneID() const { return m_iSceneID; }

	void SetNetworkOwnerID(const _llong& _llNetworkOwnerID) { this->m_llNetworkOwnerID = _llNetworkOwnerID; }
private:
	static void ServerThread(void* _pData);
private:
	virtual void Free() override;
private:
	_int														m_iSceneID;
	std::atomic_bool								m_isNetworkTickRunning;

	UOverExp											m_RecvTcpOverExp;
	SOCKET												m_ClientTcpSocket;
	SOCKET												m_ClientUdpSocket;
	WSADATA											m_WsaData{};
	
	TOTALBUFFER									m_TcpTotalBuffer;
	_llong													m_CurrentBufferLength;
	_llong													m_RemainBufferLength;
	_llong													m_llNetworkOwnerID;
	SHPTR< UNetworkAddress>		m_spNetworkAddress;
	NETWORKACTORCONTAINER		m_NetworkActorContainer;
	NETWORKINITDATACONTAINER	m_NetworkInitDataContainer;

	UOverExp*										m_pSendOverExp;

	LIST<UProcessedData>					m_ProccessedDataList;
};	

END