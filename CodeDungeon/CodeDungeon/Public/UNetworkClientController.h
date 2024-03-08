#pragma once
#include "UNetworkBaseController.h"


BEGIN(Client)

class UNetworkClientController final : public UNetworkBaseController{
public:
	UNetworkClientController();
	NO_COPY(UNetworkClientController)
	DESTRUCTOR(UNetworkClientController)
public:
	virtual HRESULT NativeConstruct(const _string& _strIPAddress, const _int _PortNumber) override;
protected:
	virtual void NativePacket() override;
	virtual void ProcessPacket(_char* _pPacket, PACKETHEAD _PacketHead) override;
private:
	virtual void Free() override;
private:

};

END