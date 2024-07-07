#pragma once
#include "UNetworkQueryProcessing.h"


BEGIN(Engine)

END

BEGIN(Client)

class CNetworkQueryProcessor final : public UNetworkQueryProcessing{
public:
	CNetworkQueryProcessor(CSHPTRREF<UNetworkBaseController> _spNetworkBaseController);
	DESTRUCTOR(CNetworkQueryProcessor)

public:
	virtual void MakeActors(const NETWORKRECEIVEINITDATA& _NetworkRecvInitData) override;
private:
#ifdef _ENABLE_PROTOBUFF
	void MakeMonster(const NETWORKRECEIVEINITDATA& _NetworkRecvInitData);
#endif
private:
	virtual void Free() override;
};

END

