#include "CoreDefines.h"
#include "UObject.h"

namespace Core {

	UObject::UObject(SHPTR<UCoreInstance> _spCoreInstance) : 
		m_wpCoreInstance{_spCoreInstance}
	{
	}

	UObject::UObject(const UObject& _rhs) : 
		m_wpCoreInstance{ _rhs.m_wpCoreInstance }
	{
	}

	void UObject::Free()
	{
	}
}