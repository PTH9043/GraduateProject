#include "ServerDefines.h"
#include "CMummyAnimController.h"
#include "AAnimator.h"
#include "AAnimation.h"
#include "APawn.h"

namespace Server {

	CMummyAnimController::CMummyAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string & _strFolderPath, const _string & _strFileName) : 
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName)
	{
	}

	void CMummyAnimController::Tick(const _double& _dTimeDelta)
	{
		__super::Tick(_dTimeDelta);
		SHPTR<APawn> spPawn = GetOwner();
		SHPTR<AAnimator> spAnimator = GetAnimator();
		SHPTR<AAnimation> spAnimation = spAnimator->GetCurAnimation();

		_string CurAnimName = spAnimation->GetAnimName();
		
		{

		}
	}

	void CMummyAnimController::Free()
	{
	}
}