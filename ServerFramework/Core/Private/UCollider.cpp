#include "CoreDefines.h"
#include "UCollider.h"

namespace Core {

	UCollider::UCollider(COLLIDERTYPE _ColliderType, const Vector3 _vOriPos,
		const Vector3 _vOriScale) : m_ColliderType{_ColliderType},
		m_vOriginPosition{ _vOriPos }, m_vOriginScale{ _vOriScale }, 
		m_mChangeMatrix{_matrix::Identity}, m_vChangeScale{}
	{
	}

	void UCollider::Tick(const _matrix _WorldMatrix)
	{
	}

	_bool UCollider::IsCollision(SHPTR<UCollider> _spCollider)
	{
		RETURN_CHECK(nullptr == _spCollider, false);

		return true;
	}

	void UCollider::UpdateChangeValue(const _matrix& _ChangeMatrix, const Vector3& _vChangeScale)
	{
		m_mChangeMatrix = _ChangeMatrix;
		m_vChangeScale = _vChangeScale;
	}

	void UCollider::Free()
	{
	}
}

