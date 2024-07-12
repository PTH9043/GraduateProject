#include "ServerDefines.h"
#include "CChestAnimController.h"

namespace Server {

	CChestAnimController::CChestAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string& _strFolderPath, const _string& _strFileName) : 
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName)
	{
	}

	void CChestAnimController::Tick(const _double& _dTimeDelta)
	{
	}

	void CChestAnimController::Free()
	{
	}


}