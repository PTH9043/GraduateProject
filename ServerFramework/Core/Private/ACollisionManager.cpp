#include "CoreDefines.h"
#include "ACollisionManager.h"
#include "APawn.h"
#include "ASession.h"

namespace Core {


	ACollisionManager::ACollisionManager() : m_CollisionPawnList{}
	{
	}

	void ACollisionManager::AddCollisionList(APawn* _pPawn)
	{
		assert(nullptr != _pPawn);
		m_CollisionPawnList.insert(_pPawn);
	}

	void ACollisionManager::CollisionSituation(const _double _dTimeDelta)
	{
		for (auto& iter : m_CollisionPawnList)
		{
			if (nullptr == iter)
				continue;

			if (true == iter->IsPermanentDisable())
				continue;

			if (false == iter->IsActive())
				continue;

			for (auto& Monster : m_CollisionPawnList)
			{
				if (nullptr == Monster)
					continue;

				if (true == Monster->IsPermanentDisable())
					continue;

				if (false == Monster->IsActive())
					continue;

				iter->IsHit(Monster, _dTimeDelta);
			}
		}
	}

	void ACollisionManager::Free()
	{
	}

}