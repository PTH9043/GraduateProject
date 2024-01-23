#include "CoreDefines.h"
#include "USpaceManager.h"
#include "USpace.h"

namespace Core {

	USpaceManager::USpaceManager() : m_spRootSpace{nullptr}
	{
	}

	void USpaceManager::BuildGameSpace(const SPACEINFO& _SpaceInfo)
	{
		m_spRootSpace = CreateInitNative<USpace>(_SpaceInfo);
		m_SpaceContainer.push_back(m_spRootSpace);
		BuildTreeRecursively(_SpaceInfo, m_spRootSpace);
	}

	void USpaceManager::InsertSessionInSpace(SHPTR<USession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		m_spRootSpace->InsertSession(_spSession);
	}

	void USpaceManager::BuildTreeRecursively(const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents)
	{
		if (0 == _SpaceInfo.Depths) {
			_spParents->OnChildNode();
			return;
		}

		Vector3 vHalfExtents = _SpaceInfo.vExtents * 0.5f;

		for (_int i = 0; i < 2; ++i) {
			for (_int j = 0; j < 2; ++j) {
				for (_int k = 0; k < 2; ++k) {
					Vector3 vChildCenter = _SpaceInfo.vCenter + Vector3(i, j, k) * vHalfExtents;
					// SPACE INFO
					SPACEINFO SpaceInfo{ vChildCenter, vHalfExtents, _SpaceInfo.Depths - 1, static_cast<_int>(m_SpaceContainer.size())};
					SHPTR<USpace> spSpace = _spParents->MakeChild(i * 4 + j * 2 + k, SpaceInfo);
					m_SpaceContainer.push_back(spSpace);
					// 재귀적으로 호출한다. 
					BuildTreeRecursively(SpaceInfo, spSpace);
				}
			}
		}
	}

	void USpaceManager::Free()
	{
	}

}