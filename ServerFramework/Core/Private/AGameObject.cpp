#include "CoreDefines.h"
#include "AGameObject.h"
#include "AAABBCollider.h"
#include "AOBBCollider.h"
#include "ASphereCollider.h"
#include "ATransform.h"
#include "ASpace.h"
#include "ACoreInstance.h"

namespace Core
{
	AGameObject::AGameObject(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		ACoreObject(OBJCON_CONDATA), m_SessionID{_ID}, m_SessionType{_SessionType}, m_SpaceIndex{0},
		m_spTransform{ nullptr }, m_spCollider{ nullptr }, m_CellIndex{0}
	{
	}

	_bool AGameObject::Start()
	{
		m_spTransform = Create<ATransform>();
		return _bool();
	}

	void AGameObject::CreateColliderAndTransform(const COLLIDERINFO& _ColliderInfo, const Vector3& _vPos)
	{
		switch (_ColliderInfo.ColliderType) {
		case COLLIDERTYPE::OBB:
			m_spCollider = Create<AOBBCollider>(_ColliderInfo.vPos, _ColliderInfo.vScale);
			break;
		case COLLIDERTYPE::AABB:
			m_spCollider = Create<AAABBCollider>(_ColliderInfo.vPos, _ColliderInfo.vScale);
			break;
		case COLLIDERTYPE::SPHERE:
			m_spCollider = Create<ASphereCollider>(_ColliderInfo.vPos, _ColliderInfo.vScale);
			break;
		}
		m_spTransform = Create<ATransform>();
		m_spTransform->SetPos(_vPos);
	}

	void AGameObject::BringSpaceIndex(SHPTR<ASpace> _spSpace)
	{
		RETURN_CHECK(nullptr == _spSpace, ;);
		m_SpaceIndex = _spSpace->GetSpaceIndex();
	}

	void AGameObject::Free()
	{
	}
}