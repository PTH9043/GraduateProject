#pragma once
#include "UNetworkBaseController.h"


BEGIN(Client)
class CMainCamera;

class CNetworkClientController final : public UNetworkBaseController{
public:
	CNetworkClientController();
	NO_COPY(CNetworkClientController)
	DESTRUCTOR(CNetworkClientController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) override;
	virtual void MakeActors() override;
protected:
	virtual void NativePacket() override;
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) override;
private:
	virtual void Free() override;
private:

};

END