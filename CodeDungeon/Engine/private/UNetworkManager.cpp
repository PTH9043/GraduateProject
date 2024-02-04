#include "EngineDefine.h"
#include "UNetworkManager.h"
#include "UNetworkBaseController.h"

UNetworkManager::UNetworkManager() : m_spNetworkBaseController{nullptr}
{
}

HRESULT UNetworkManager::StartNetwork(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController)
{
	m_spNetworkBaseController = _spNetworkBaseController;
	return S_OK;
}

void UNetworkManager::InsertProcessedDataToContainer(void* _pData, size_t _Size, _int _DataType)
{
	m_ProcessedDataContainer.push(UProcessedData{ _pData, _Size, _DataType });
}

void UNetworkManager::PopProcessedData(POINTER_IN UProcessedData* _pData)
{
	RETURN_CHECK(false == m_ProcessedDataContainer.empty(), ;);
	RETURN_CHECK(nullptr == _pData, ;);
	m_ProcessedDataContainer.try_pop(*_pData);
}

void UNetworkManager::SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize)
{
	RETURN_CHECK(nullptr == m_spNetworkBaseController, ;);
	m_spNetworkBaseController->SendTcpPacket(_pPacket, _PacketType, _PacketSize);
}

void UNetworkManager::Free()
{
	m_spNetworkBaseController.reset();
}
