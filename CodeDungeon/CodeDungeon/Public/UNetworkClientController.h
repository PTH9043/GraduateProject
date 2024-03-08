#pragma once
#include "UNetworkBaseController.h"


BEGIN(Engine)
class UCharacter;
END

BEGIN(Client)

/*
@ Date: 2024-02-25, Writer: 박태현
@ Explain
- Client에서 Network를 관리하는 클래스이다. 
*/
class UNetworkClientController final : public UNetworkBaseController{
	using NETWORKACTORCONTAINER = UNORMAP<_llong, SHPTR<UCharacter>>;
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
	NETWORKACTORCONTAINER		m_NetworkCharacterContainer;
};

END