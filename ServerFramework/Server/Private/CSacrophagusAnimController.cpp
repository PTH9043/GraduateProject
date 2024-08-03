#include "ServerDefines.h"
#include "CSacrophagusAnimController.h"
#include "APawn.h"
#include "AAnimator.h"
#include "AAnimation.h"

namespace Server {

	CSacrophagusAnimController::CSacrophagusAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix) :
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName, _PivotMatrix),
		m_iOwnerType{ 0 }, m_dSarcophagusOpeningSpeed {20}, m_dLyingSarcophagusTimeArcOpenStart{ 50 },
		m_dLyingSarcophagusTimeArcOpenEnd{ 50 }, m_dStandingSarcophagusTimeArcOpenEnd{ 30 }
	{
		m_iOwnerType = _spPawn->GetGameObjectType();
	}

	void CSacrophagusAnimController::Tick(const _double& _dTimeDelta)
	{

	}

	void CSacrophagusAnimController::Free()
	{
	}


}