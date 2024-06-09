#pragma once 

#include "UBase.h"
#include "UProcessedData.h"

BEGIN(Engine)
class UNetworkBaseController;
class UActor;

/*
@ Date: 2024-02-03,  Writer: 박태현
@ Explain
- Network Data들을 모으고 처리하기 위한 Manager
*/
class UNetworkManager final : public UBase {
public:
	UNetworkManager();
	NO_COPY(UNetworkManager)
	DESTRUCTOR(UNetworkManager)
public:
	HRESULT StartNetwork(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController);
	void InsertProcessedDataToContainer(void* _pData, size_t _Size, _int _DataType);
	void PopProcessedData(POINTER_IN UProcessedData* _pData);

	void SendTcpPacket(_char* _pPacket, _short _PacketType, _short _PacketSize);
private:
	virtual void Free() override;
private:
	SHPTR<UNetworkBaseController>		m_spNetworkBaseController;
	CONQUEUE<UProcessedData>				m_ProcessedDataContainer;


};

END

