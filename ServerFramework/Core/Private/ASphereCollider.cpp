#include "CoreDefines.h"
#include "ASphereCollider.h"

namespace Core {

	ASphereCollider::ASphereCollider(const Vector3 _vOriPos, const Vector3 _vOriScale) : 
		ACollider(COLLIDERTYPE::SPHERE, _vOriPos, _vOriScale)
	{
	}

	void ASphereCollider::Tick(const _matrix _WorldMatrix)
	{
		Vector3 vPos = GetOriginPosition();
		Vector3 vScale = GetOriginScale();

		vPos = PTH::TransformCoord(vPos, _WorldMatrix);
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			UpdateChangeValue(glm::translate(vPos), vScale * _WorldMatrix.GetScale());
		}
	}

	_bool ASphereCollider::IsCollision(SHPTR<ACollider> _spCollider)
	{
		RETURN_CHECK(__super::IsCollision(_spCollider), false);

		_matrix ChangeMatrix = GetChangeMatrix();
		_matrix TargetMatrix = _spCollider->GetChangeMatrix();

		Vector3 vChangePos = ChangeMatrix.GetPos();
		Vector3 vChangeScale = GetChangeScale();

		Vector3 vTargetPos = TargetMatrix.GetPos();
		Vector3 vTargetScale = _spCollider->GetChangeScale();

		_matrix TargetMatrixInv = TargetMatrix.Invert();

		Vector3 vSphereLocalPos = PTH::TransformCoord(vChangePos - vTargetPos, TargetMatrixInv);
		Vector3 vDistance = glm::abs(vSphereLocalPos) - 0.5f * vChangeScale;
		_float MinDistance = glm::max(vDistance.x, glm::max(vDistance.y, vDistance.z));

		return MinDistance <= vTargetScale.x;
	}

	void ASphereCollider::Free()
	{
	}

}