#pragma once 

#include "ServerUtility.h"

BEGIN(Engine)

class UNetworkManager final : public UBase {
public:
	UNetworkManager();
	NO_COPY(UNetworkManager)
	DESTRUCTOR(UNetworkManager)
public:
	HRESULT ReadyNetwork(const _wstring& _wstrIPAddress, const _int _PortNumber);
private:
	virtual void Free() override;
private:
	SHPTR<UNetworkAddress>	m_spNetworkAddress;
};

END

