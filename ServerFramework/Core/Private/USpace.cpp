#include "CoreDefines.h"
#include "USpace.h"
#include "USession.h"

namespace Core {

	USpace::USpace()
	{
	}

	void USpace::InsertSession(SHPTR<UCollider> _spCollider, SHPTR<USession> _spSession)
	{
		for (auto& iter : m_SessionContainer)
		{
			
		}
	}

	void USpace::Rebalance(SHPTR<UCollider> _spCollider, SHPTR<USession> _spSession)
	{
	}

	void USpace::Free()
	{
	}

}