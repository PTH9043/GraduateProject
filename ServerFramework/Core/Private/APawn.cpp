#include "CoreDefines.h"
#include "APawn.h"

namespace Core {

	APawn::APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		AGameObject(OBJCON_CONDATA, _ID, _SessionType), m_spAnimController{nullptr}
	{
	}

	void APawn::Free()
	{
	}

}