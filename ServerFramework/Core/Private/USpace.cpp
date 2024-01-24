#include "CoreDefines.h"
#include "USpace.h"
#include "USession.h"
#include "UAABBCollider.h"

namespace Core {

	USpace::USpace() : m_SpaceIndex{0}, m_DepthLevel{0}, m_isChildNode{false}, m_wpParents{}, m_SpaceChild{},
	m_spCollider{ nullptr }, m_SessionContainer{}
	{
		m_SessionContainer.Initialize(TLS::MAX_THREAD, TLS::MAX_THREAD * 5);
	}

	_bool USpace::NativeConstruct(const SPACEINFO& _SpaceInfo, SHPTR<USpace> _spParents)
	{
		m_wpParents = _spParents;
		m_spCollider = Create<UAABBCollider>(_SpaceInfo.vCenter, _SpaceInfo.vExtents);
		m_DepthLevel = _SpaceInfo.Depths;
		m_SpaceIndex = _SpaceInfo.Index;
		return true;
	}

	_bool USpace::InsertSession(SHPTR<USession> _spSession)
	{
		return InsertSessionRecursively(_spSession);
	}

	void USpace::BroadCastMessage(_char* _pPacket, const PACKETHEAD& _PacketHead)
	{
		for (auto& iter : m_SessionContainer) {
			iter->WriteData(_pPacket, _PacketHead);
		}
	}

	_int USpace::GetChildIndexToPosition(const Vector3 _vPos)
	{
		_int index = 0;
		index |= (m_spCollider->GetOriginPosition().x > _vPos.x) ? 1 : 0;
		index |= (m_spCollider->GetOriginPosition().y > _vPos.y) ? 2 : 0;
		index |= (m_spCollider->GetOriginPosition().z > _vPos.z) ? 4 : 0;
		return index;
	}

	_bool USpace::IsContains(SHPTR<UCollider> _spCollider)
	{
		RETURN_CHECK(nullptr == _spCollider, false);
		return m_spCollider->IsCollision(_spCollider);
	}

	SHPTR<USpace> USpace::MakeChild(const _int _Index, const SPACEINFO& _SpaceInfo)
	{
		RETURN_CHECK(MAX_OCTREENODE_LENGTH <= _Index, nullptr);
		if (m_SpaceChild.size() <= 0) {
			m_SpaceChild.resize(MAX_OCTREENODE_LENGTH);
		}
		m_SpaceChild[_Index] = CreateInitNative<USpace>(_SpaceInfo, ThisShared<USpace>());
		return m_SpaceChild[_Index];
	}

	_bool USpace::InsertSessionRecursively(CSHPTRREF<USession> _spSession)
	{
		RETURN_CHECK(false == m_spCollider->IsCollision(_spSession->GetCollider()), false);
		// 재귀 함수로 Insert 한다. 
		if (true == m_isChildNode) {
			_spSession->BringSpaceIndex(ThisShared<USpace>());
			m_SessionContainer.Insert(_spSession);
		}
		else
		{
			return InsertSessionRecursively(_spSession);
		}
		return true;
	}

	void USpace::Free()
	{
	}

}