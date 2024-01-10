#pragma once

#include "UApp.h"

BEGIN(Core)
class UCoreInstance;
END

class CClientAdiminstor;

class CClientApp final : public UApp {
public:
	CClientApp();
	NO_COPY(CClientApp)
public:
	virtual _bool Start() override;
private:
};

