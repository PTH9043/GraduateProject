#include "CoreDefines.h"
#include "USpace.h"
#include "USession.h"
#include "UAABBCollider.h"
namespace Core {

	USpace::USpace() :  m_wpParents{}, m_SpaceChild{},
	m_spCollider{ nullptr }, m_SessionContainer{}
	{
		m_SessionContainer.Initialize(TLS::MAX_THREAD, TLS::MAX_THREAD * 5);
	}

	_bool USpace::NativeConstruct(REF_IN _int& _CurIndex, const _int _MaxCnt, const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents)
	{
		m_spCollider = Create<UAABBCollider>(_SpaceInfo.vCenter, _SpaceInfo.vExtents);

		if (_MaxCnt <= _CurIndex)
		{
			return true;
		}
		m_SpaceChild.resize(MAX_OCTREENODE_LENGTH);
		_CurIndex += MAX_OCTREENODE_LENGTH;

		Vector3 HalfExtents = _SpaceInfo.vExtents / 2.f;
		// Index: 0
		{
			Vector3 ChildCenter = _SpaceInfo.vCenter - HalfExtents;
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[0] = CreateInitNative<USpace>(REF_OUT _CurIndex , _MaxCnt, spaceInfo);
		}
		// Index: 1
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x + HalfExtents.x, _SpaceInfo.vCenter.y - HalfExtents.y, _SpaceInfo.vCenter.z - HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents};
			m_SpaceChild[1] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 2
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x - HalfExtents.x, _SpaceInfo.vCenter.y + HalfExtents.y, _SpaceInfo.vCenter.z - HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[2] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 3
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x + HalfExtents.x, _SpaceInfo.vCenter.y + HalfExtents.y, _SpaceInfo.vCenter.z - HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[3] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 4
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x - HalfExtents.x, _SpaceInfo.vCenter.y - HalfExtents.y, _SpaceInfo.vCenter.z + HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[4] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 5
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x + HalfExtents.x, _SpaceInfo.vCenter.y - HalfExtents.y, _SpaceInfo.vCenter.z + HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[5] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 6
		{
			Vector3 ChildCenter{ _SpaceInfo.vCenter.x - HalfExtents.x, _SpaceInfo.vCenter.y + HalfExtents.y, _SpaceInfo.vCenter.z + HalfExtents.z };
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[6] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		// Index: 7
		{
			Vector3 ChildCenter = _SpaceInfo.vCenter + HalfExtents;
			SPACEINFO spaceInfo{ ChildCenter, HalfExtents };
			m_SpaceChild[7] = CreateInitNative<USpace>(REF_OUT _CurIndex, _MaxCnt, spaceInfo);
		}
		return true;
	}

	void USpace::InsertSession(SHPTR<USession> _spSession)
	{

	}

	void USpace::Rebalance()
	{
	}

	void USpace::Free()
	{
	}

}