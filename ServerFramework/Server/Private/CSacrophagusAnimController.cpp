#include "ServerDefines.h"
#include "CSacrophagusAnimController.h"
#include "APawn.h"

namespace Server {

	CSacrophagusAnimController::CSacrophagusAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string& _strFolderPath, const _string& _strFileName) : 
		AAnimController(OBJCON_CONDATA, _spPawn, _strFolderPath, _strFileName),
		m_iOwnerType{ 0 }, m_dSarcophagusOpeningSpeed {20}, m_dLyingSarcophagusTimeArcOpenStart{ 50 },
		m_dLyingSarcophagusTimeArcOpenEnd{ 50 }, m_dStandingSarcophagusTimeArcOpenEnd{ 30 }
	{
		m_iOwnerType = _spPawn->GetGameObjectType();
	}

	void CSacrophagusAnimController::Tick(const _double& _dTimeDelta)
	{
		if (TAG_CHAR::TAG_SARCOPHAGUS_LAYING == m_iOwnerType)
		{
			if (GetElapsedTime() < LyingSarcophagusTimeArcOpenEnd)
				GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, LyingSarcophagusTimeArcOpenStart + GetElapsedTime());
		}
		else
		{
			if (GetElapsedTime() < StandingSarcophagusTimeArcOpenEnd)
				GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, GetElapsedTime());
		}
	}

	void CSacrophagusAnimController::Free()
	{
	}


}