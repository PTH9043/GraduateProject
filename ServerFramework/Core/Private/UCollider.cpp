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

	void UCollider::AABBTick(const _matrix& _WorldMatrix)
	{
		Vector3 vPos = GetOriginPosition();
		Vector3 vScale = GetOriginScale();

		vPos = PTH::TransformCoord(vPos, _WorldMatrix);
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			m_mChangeMatrix = glm::translate(vPos);
			m_vChangeScale = vScale * _WorldMatrix.GetScale();
		}
	}

	void UCollider::OBBTick(const _matrix& _WorldMatrix)
	{
		Vector3 vPos = GetOriginPosition();
		Vector3 vScale = GetOriginScale();
		vPos = PTH::TransformCoord(vPos, _WorldMatrix);
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			m_vChangeScale = m_vOriginScale * _WorldMatrix.GetScale();
			m_mChangeMatrix = _WorldMatrix;
			m_mChangeMatrix.SetPos(vPos);
		}
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

