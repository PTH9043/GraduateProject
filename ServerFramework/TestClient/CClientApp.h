#pragma once

#include "UBase.h"

BEGIN(Core)
class UCoreInstance;
END

class CClientAdiminstor;

class CClientApp final : public UBase {
public:
	CClientApp();
	NO_COPY(CClientApp)
	DESTRUCTOR(CClientApp)
public:
	void Init();
private:
	virtual void Free() override;
	
private:
	SHPTR<UCoreInstance>				m_spCoreInstance;
	SHPTR<CClientAdiminstor>		m_spClientAdiminstor;
};

