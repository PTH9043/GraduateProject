#include "CoreDefines.h"
#include "ACollider.h"

namespace Core {

	ACollider::ACollider(COLLIDERTYPE _ColliderType, const Vector3 _vOriPos,
		const Vector3 _vOriScale) : m_ColliderType{_ColliderType},
		m_vOriginPosition{ _vOriPos }, m_vScale{ _vOriScale }, 
		m_mChangeMatrix{_matrix::Identity}, m_vChangeScale{}
	{
	}

	void ACollider::Tick(const _matrix _WorldMatrix)
	{
	}

	_bool ACollider::IsCollision(SHPTR<ACollider> _spCollider)
	{
		RETURN_CHECK(nullptr == _spCollider, false);

		return true;
	}

	void ACollider::UpdateChangeValue(const _matrix& _ChangeMatrix, const Vector3& _vChangeScale)
	{
		m_mChangeMatrix = _ChangeMatrix;
		m_vChangeScale = _vChangeScale;
	}

	void ACollider::Free()
	{
	}
}

