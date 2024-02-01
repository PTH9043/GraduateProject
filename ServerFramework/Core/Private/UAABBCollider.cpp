#include "CoreDefines.h"
#include "UAABBCollider.h"

namespace Core {

	UAABBCollider::UAABBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale) : 
		ACollider(COLLIDERTYPE::AABB, _vOriPos, _vOriScale)
	{
	}

	void UAABBCollider::Tick(const _matrix _WorldMatrix)
	{
		Vector3 vPos = GetOriginPosition();
		Vector3 vScale = GetOriginScale();

		vPos = PTH::TransformCoord(vPos, _WorldMatrix);
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			UpdateChangeValue(glm::translate(vPos), vScale * _WorldMatrix.GetScale());
		}
	}

	_bool UAABBCollider::IsCollision(SHPTR<ACollider> _spCollider)
	{
		RETURN_CHECK(__super::IsCollision(_spCollider), false);
		_matrix ChangeMatrix = GetChangeMatrix();
		_matrix TargetMatrix = _spCollider->GetChangeMatrix();

		if (_spCollider->GetColliderType() == COLLIDERTYPE::AABB)
		{
			Vector3 vChangePos = ChangeMatrix.GetPos();
			Vector3 vChangeScale = GetChangeScale();

			Vector3 vTargetPos = TargetMatrix.GetPos();
			Vector3 vTargetScale = _spCollider->GetChangeScale();

			// 현재 AABB의 최소와 최대 좌표 계산
			Vector3 myMin = vChangePos - 0.5f * vChangeScale;
			Vector3 myMax = vChangePos + 0.5f * vChangeScale;

			// 대상 AABB의 최소와 최대 좌표 계산
			Vector3 otherMin = vTargetPos - 0.5f * vTargetScale;
			Vector3 otherMax = vTargetPos + 0.5f * vTargetScale;

			// AABB 간의 충돌 검사
			if (myMax.x < otherMin.x || myMin.x > otherMax.x) return false;
			if (myMax.y < otherMin.y || myMin.y > otherMax.y) return false;
			if (myMax.z < otherMin.z || myMin.z > otherMax.z) return false;

			return true; // AABB 간의 충돌이 있음
		}
		else if (_spCollider->GetColliderType() == COLLIDERTYPE::OBB)
		{
			Vector3 vChangePos = ChangeMatrix.GetPos();
			Vector3 vChangeScale = GetChangeScale();

			Vector3 vTargetPos = TargetMatrix.GetPos();
			Vector3 vTargetScale = _spCollider->GetChangeScale();

			glm::mat3 TargetInvMatrix = glm::mat3(TargetMatrix.Invert().m);

			Vector3 vAABBLocalPos = Vector3(TargetInvMatrix *
				(vChangePos - vTargetPos));
			Vector3 vAABBScale = Vector3(TargetInvMatrix * vChangeScale);
			// 3축 검사
			for (_int i = 0; i < AXIS_CNT; ++i)
			{
				Vector3 Axis = TargetMatrix.m[i];
				_float ProjectionAABB = glm::abs(glm::dot(vAABBLocalPos, Axis));
				_float ProjectionOBB = glm::abs(glm::dot(vTargetScale, Axis));

				if (ProjectionAABB + vAABBLocalPos[i] < ProjectionOBB) {
					return false;
				}
			}
			return true;
		}
		else if (_spCollider->GetColliderType() == COLLIDERTYPE::SPHERE)
		{
			Vector3 vChangePos = ChangeMatrix.GetPos();
			Vector3 vChangeScale = GetChangeScale();

			Vector3 vTargetPos = TargetMatrix.GetPos();
			Vector3 vTargetScale = _spCollider->GetChangeScale();

			Vector3 vDistance = glm::abs(vChangePos - vTargetPos) - 0.5f * vChangeScale;
			float minDistance = glm::max(vDistance.x, glm::max(vDistance.y, vDistance.z));

			return minDistance <= vTargetScale.x;
		}
		return false;
	}

	void UAABBCollider::Free()
	{
	}

}