#pragma once
#include "UNetworkBaseController.h"


BEGIN(Client)
class CMainCamera;

class CNetworkClientController final : public UNetworkBaseController{
	enum
	{
		MAINCAMERA_ACTORS_ID = 0,
	};
public:
	CNetworkClientController();
	NO_COPY(CNetworkClientController)
	DESTRUCTOR(CNetworkClientController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) override;
	virtual void MakeActors(const VECTOR<SHPTR<UActor>>& _actorContainer) override;
	virtual void CreateNetworkActor(_int _NetworkID, const NETWORKRECEIVEINITDATA& _networkInitData) override;
protected:
	virtual void NativePacket() override;
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) override;
private:
	virtual void Free() override;
private:
	_string m_strTriggerName;
};

END