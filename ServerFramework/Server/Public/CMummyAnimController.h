#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H

#include "AAnimController.h"

BEGIN(Server)

class CMummyAnimController final : public AAnimController {
public:
	CMummyAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn,
		const _string& _strFolderPath, const _string& _strFileName);
	DESTRUCTOR(CMummyAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) override;
private:
	virtual void Free() override;
private:
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CMUMMYANIMCONTROLLER_H