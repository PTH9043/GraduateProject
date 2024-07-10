#include "CoreDefines.h"
#include "APawn.h"
#include "AAnimController.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		AGameObject(OBJCON_CONDATA, _ID, _SessionType), m_spAnimController{nullptr}
	{
	}

	void APawn::SetActive(const _bool _isActive)
	{
		__super::SetActive(_isActive);
		if (nullptr != m_spAnimController)
		{
			m_spAnimController->SetOwnerPawnActiveStrong(_isActive);
		}
	}

	void APawn::Free()
	{
	}

}