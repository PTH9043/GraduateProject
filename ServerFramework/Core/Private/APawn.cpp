#include "CoreDefines.h"
#include "APawn.h"
#include "AAnimController.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		AGameObject(OBJCON_CONDATA, _ID, _SessionType), m_spAnimController{nullptr}
	{
	}

	void APawn::SetActiveWeak(const _bool _isActive)
	{
		__super::SetActiveWeak(_isActive);

		if (nullptr != m_spAnimController)
		{
			m_spAnimController->SetOwnerPawnActiveWeak(_isActive);
		}
	}

	void APawn::SetActiveStrong(const _bool _isActive)
	{
		__super::SetActiveStrong(_isActive);
		if (nullptr != m_spAnimController)
		{
			m_spAnimController->SetOwnerPawnActiveStrong(_isActive);
		}
	}

	void APawn::Free()
	{
	}

}