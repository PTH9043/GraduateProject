#include "EngineDefine.h"
#include "UNetworkManager.h"

UNetworkManager::UNetworkManager() : m_spNetworkAddress{nullptr}
{
}

HRESULT UNetworkManager::ReadyNetwork(const _wstring& _wstrIPAddress, 
	const _int _PortNumber)
{

	return S_OK;
}

void UNetworkManager::Free()
{
}
