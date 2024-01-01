#include "CoreDefines.h"
#include "UBase.h"

namespace Core
{
	UBase::UBase() : Core::UEnableSharedFromThis<UBase>()
	{}

	UBase::UBase(const UBase& _rhs) 
	{

	}

	UBase::~UBase()
	{

	}
}