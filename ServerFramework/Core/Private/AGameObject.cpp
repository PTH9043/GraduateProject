#include "CoreDefines.h"
#include "AGameObject.h"
#include "ACollider.h"
#include "ATransform.h"
#include "ASpace.h"
#include "ACoreInstance.h"

namespace Core
{
	AGameObject::AGameObject(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType) : 
		ACoreObject(OBJCON_CONDATA), m_SessionID{_ID}, m_SessionType{_SessionType}, m_SpaceIndex{0},
		m_spTransform{ nullptr }, m_spCollider{ nullptr }, m_CellIndex{0}, m_fMoveSpeed{0.f}, m_fRunSpeed{0.f}
	{
	}

	_bool AGameObject::Start()
	{
		m_spTransform = Create<ATransform>();
		return true;
	}

	void AGameObject::CreateColliderAndTransform(const COLLIDERINFO& _ColliderInfo, const Vector3& _vPos)
	{
		m_spCollider = Create<ACollider>((ACollider::TYPE)_ColliderInfo.iColliderType, ACollider::COLLIDERDESC{ _ColliderInfo.vPos, _ColliderInfo.vScale });
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