
#pragma once
#include "UAnimationController.h"

BEGIN(Client)
class CSarcophagus;

class CSarcophagusAnimController final : public UAnimationController {
public:
	CSarcophagusAnimController(CSHPTRREF<UDevice> _spDevice);
	CSarcophagusAnimController(const CSarcophagusAnimController& _rhs);
	virtual ~CSarcophagusAnimController() = default;
public:
	virtual void Free() override;
	CLONE_MACRO(CSarcophagusAnimController, "CSarcophagusAnimController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta) override;
	// 네트워크 정보를 받아오는 함수
	virtual void ReceiveNetworkProcessData(void* _pData) override;
private:
	WKPTR< CSarcophagus>			m_wpSarcophagusMob;
	_double											m_dRecvAnimDuration;
};

END
