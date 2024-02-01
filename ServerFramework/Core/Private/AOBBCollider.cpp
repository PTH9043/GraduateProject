#include "CoreDefines.h"
#include "AOBBCollider.h"

namespace Core {

	AOBBCollider::AOBBCollider(const Vector3 _vOriPos, const Vector3 _vOriScale) : 
		ACollider(COLLIDERTYPE::OBB, _vOriPos, _vOriScale)
	{
	}

	void AOBBCollider::Tick(const _matrix _WorldMatrix)
	{
		Vector3 vPos = GetOriginPosition();
		Vector3 vScale = GetOriginScale();

		vPos = PTH::TransformCoord(vPos, _WorldMatrix);
		_WorldMatrix.SetPos(vPos);
		{
			std::atomic_thread_fence(std::memory_order_seq_cst);
			UpdateChangeValue(_WorldMatrix, vScale * _WorldMatrix.GetScale());
		}
	}

	_bool AOBBCollider::IsCollision(SHPTR<ACollider> _spCollider)
	{
		RETURN_CHECK(__super::IsCollision(_spCollider), false);

		_matrix ChangeMatrix = GetChangeMatrix();
		_matrix TargetMatrix = _spCollider->GetChangeMatrix();

		if (COLLIDERTYPE::AABB == _spCollider->GetColliderType())
		{
			Vector3 vChangePos = ChangeMatrix.GetPos();
			Vector3 vChangeScale = GetChangeScale();

			Vector3 vTargetPos = TargetMatrix.GetPos();
			Vector3 vTargetScale = _spCollider->GetChangeScale();

			glm::mat3 ChangeInvMatrix = glm::mat3(ChangeMatrix.Invert().m);

			Vector3 vAABBLocalPos = Vector3(ChangeInvMatrix *
				(vTargetPos - vChangePos));
			Vector3 vAABBScale = Vector3(ChangeInvMatrix * vTargetScale);
			// 3축 검사
			for (_int i = 0; i < AXIS_CNT; ++i)
			{
				Vector3 Axis = ChangeMatrix.m[i];
				_float ProjectionAABB = glm::abs(glm::dot(vAABBLocalPos, Axis));
				_float ProjectionOBB = glm::abs(glm::dot(vChangeScale, Axis));

				if (ProjectionAABB + vAABBLocalPos[i] < ProjectionOBB) {
					return false;
				}
			}
			return true;
		}
		else if (COLLIDERTYPE::OBB == _spCollider->GetColliderType())
		{
			Vector3 ChangePos = ChangeMatrix.GetPos();
			Vector3 ChangeScale = GetChangeScale();

			Vector3 TargetPos = TargetMatrix.GetPos();
			Vector3 TargetScale = _spCollider->GetChangeScale();

			ARRAY<Vector3, AXIS_CNT>					OBBAxis1{ ChangeMatrix.m[0] , ChangeMatrix.m[1] , ChangeMatrix.m[2] };
			ARRAY<Vector3, AXIS_CNT>					OBBAxis2{ TargetMatrix.m[0] , TargetMatrix.m[1] , TargetMatrix.m[2] };

			ARRAY<Vector3, PROJ_AXIS_CNT>		ProjectionAxis
			{
					ChangeMatrix.m[0] , ChangeMatrix.m[1] , ChangeMatrix.m[2], 
					TargetMatrix.m[0] , TargetMatrix.m[1] , TargetMatrix.m[2], 
					glm::cross(ProjectionAxis[0], ProjectionAxis[4]), glm::cross(ProjectionAxis[0], ProjectionAxis[5]), 
					glm::cross(ProjectionAxis[0], ProjectionAxis[6]), glm::cross(ProjectionAxis[1], ProjectionAxis[4]),
					glm::cross(ProjectionAxis[1], ProjectionAxis[5]), glm::cross(ProjectionAxis[1], ProjectionAxis[6]),
					glm::cross(ProjectionAxis[2], ProjectionAxis[4]), glm::cross(ProjectionAxis[2], ProjectionAxis[5]),
					glm::cross(ProjectionAxis[2], ProjectionAxis[6])
			};
			// 15 개의 축에 대하여 검사한다.
			for (_int i = 0; i < 15; ++i)
			{
				Vector3 Axis = glm::normalize(ProjectionAxis[i]);

				_float Proj1 = ProjectionOBBToAxis(ChangePos, ChangeScale, Axis);
				_float Proj2 = ProjectionOBBToAxis(TargetPos, TargetScale, Axis);

				if (Proj1 + Proj2 < glm::dot(OBBAxis1[i % 3], Axis) + glm::dot(OBBAxis2[i % 3], Axis)) {
					return false;
				}
			}
			return true;
		}
		else if (COLLIDERTYPE::SPHERE == _spCollider->GetColliderType())
		{
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
		return false;
	}

	_float AOBBCollider::ProjectionOBBToAxis(const Vector3& _vPos, const Vector3& _vScale,
		const Vector3& _vAxis)
	{
		Vector3 vHalfScale = _vScale * 0.5f;

		_float Projection = glm::dot(_vPos, _vAxis);

		Projection += vHalfScale.x * glm::abs(glm::dot(_vAxis, glm::vec3(_vScale.x, 0.0f, 0.0f)));
		Projection += vHalfScale.y * glm::abs(glm::dot(_vAxis, glm::vec3(0.0f, _vScale.y, 0.0f)));
		Projection += vHalfScale.z * glm::abs(glm::dot(_vAxis, glm::vec3(0.0f, 0.0f, _vScale.z)));

		return Projection;
	}

	void AOBBCollider::Free()
	{
	}

}