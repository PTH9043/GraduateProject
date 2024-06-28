#pragma once
#include "UBase.h"
#include "UProcessedData.h"

BEGIN(Engine)
class UNetworkBaseController;

using NETWORJOBKQUERY = LIST<UProcessedData>;
using NETWORKJOBQUERYCONTAINER = ARRAY<NETWORJOBKQUERY, MAX_THREAD_CNT>;

using NETWORKINITQUERY= LIST<NETWORKRECEIVEINITDATA>;
using NETWORKINITQUERYCONTAINER = ARRAY<NETWORKINITQUERY, MAX_THREAD_CNT>;

class UNetworkQueryProcessing abstract : public UBase {
public:
	UNetworkQueryProcessing(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController);
	DESTRUCTOR(UNetworkQueryProcessing)
public:
	void InsertQueryData(UProcessedData&& _ProcessedData);
	void ProcessQueryData();

	void InsertNetworkInitData(NETWORKRECEIVEINITDATA _NetworkRecvInitData);
	void ProcessNetworkInitData();

	virtual void MakeActors(const NETWORKRECEIVEINITDATA& _NetworkRecvInitData) PURE;
public:
	SHPTR<UNetworkBaseController> GetNetworkBaseController()  { return m_wpNetworkBaseController.lock(); }
private:
	virtual void Free() override;
private:
	NETWORKJOBQUERYCONTAINER			m_NetworkJobQueryContainer;
	NETWORKINITQUERYCONTAINER			m_NetworkInitQueryContainer;

	std::atomic_int											m_NetworkJobQueryIndex;
	std::atomic_int											m_NetworkInitQueryIndex;

	WKPTR<UNetworkBaseController>		m_wpNetworkBaseController;
};

END
