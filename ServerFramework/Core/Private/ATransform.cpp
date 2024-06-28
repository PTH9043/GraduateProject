#include "CoreDefines.h"
#include "ATransform.h"

namespace Core {

	ATransform::ATransform() : 
		m_mWorldMatrix{_matrix::Identity}, 
		m_Rotation{}, 
		m_vScale{1.f, 1.f, 1.f},
		m_vVelocity{ 0.0f, 0.0f, 0.0f },
		m_vJumpvelocity{ 0.0f, 12.f, 0.0f },
		m_vGravity{ 0.0f, -9.81f, 0.0f }
	{
	}

	const Vector3 ATransform::GetAngles() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return Vector3(glm::degrees(vEuler.x), glm::degrees(vEuler.y), glm::degrees(vEuler.z));
	}

	const _float ATransform::GetXAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.x);
	}

	const _float ATransform::GetYAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.y);
	}

	const _float ATransform::GetZAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.z);
	}

	void ATransform::SetScale(const Vector3 _vScale)
	{
		if (m_vScale != _vScale)
		{
			m_vScale = _vScale;
			std::atomic_thread_fence(std::memory_order_seq_cst);
			m_mWorldMatrix.SetScale(m_vScale);
		}
	}

	void ATransform::RotateFix(const Vector3 _vStandardAngle, const _float _TurnAnge)
	{
		m_Rotation = _quaternion::CreateFromAxisAngle(_vStandardAngle, _TurnAnge);

		_matrix RotationMatrix = _matrix::CreateFromQuaternion(m_Rotation);

		Vector3 vScale = GetScale();

		SetRight(PTH::TransformNormal(Vector3{ vScale.x, 0.f, 0.f }, RotationMatrix));
		SetUp(PTH::TransformNormal(Vector3{ 0.f, vScale.y, 0.f }, RotationMatrix));
		SetLook(PTH::TransformNormal(Vector3{ 0.f, 0.f, vScale.z }, RotationMatrix));
	}

	void ATransform::RotateFix(const Vector3 _vAngle)
	{
		Vector3 vAngle;
		vAngle.x = glm::radians(_vAngle.x);
		vAngle.y = glm::radians(_vAngle.y);
		vAngle.z = glm::radians(_vAngle.z);

		m_Rotation = _quaternion::CreateFromYawPitchRoll(vAngle);

		_matrix RotationMatrix = _matrix::CreateFromQuaternion(m_Rotation);

		SetRight(PTH::TransformNormal(Vector3{ 1.f, 0.f, 0.f }, RotationMatrix));
		SetUp(PTH::TransformNormal(Vector3{ 0.f, 1.f, 0.f }, RotationMatrix));
		SetLook(PTH::TransformNormal(Vector3{ 0.f, 0.f, 1.f }, RotationMatrix));
	}

	void ATransform::RotateTurn(const Vector3 _vAxis, const _float _Angle)
	{
		m_Rotation = _quaternion::CreateFromAxisAngle(_vAxis, _Angle);
		_matrix RotationMatrix = _matrix::CreateFromQuaternion(m_Rotation);

		Vector3 vScale = GetScale();

		Vector3 vLook = glm::normalize(GetLook()) * vScale.x;
		Vector3 vRight = glm::normalize(GetRight()) * vScale.y;
		Vector3 vUp = glm::normalize(GetUp()) * vScale.z;

		SetRight(PTH::TransformNormal(vRight, RotationMatrix));
		SetUp(PTH::TransformNormal(vUp, RotationMatrix));
		SetLook(PTH::TransformNormal(vLook, RotationMatrix));
	}

	void ATransform::RotateTurn(const Vector3 _vAngle)
	{
		m_Rotation = _quaternion::CreateFromYawPitchRoll(_vAngle);
		_matrix RotationMatrix = _matrix::CreateFromQuaternion(m_Rotation);

		Vector3 vScale = GetScale();

		Vector3 vLook = glm::normalize(GetLook()) * vScale.x;
		Vector3 vRight = glm::normalize(GetRight()) * vScale.y;
		Vector3 vUp = glm::normalize(GetUp()) * vScale.z;

		SetRight(PTH::TransformNormal(vRight, RotationMatrix));
		SetUp(PTH::TransformNormal(vUp, RotationMatrix));
		SetLook(PTH::TransformNormal(vLook, RotationMatrix));
	}

	void ATransform::LookAt(const Vector3 _vTargetPos)
	{
		m_mWorldMatrix.LookAt(_vTargetPos);
	}


	const _float ATransform::ComputeDistance(const Vector3 _vPos)
	{
		return m_mWorldMatrix.ComputeDistance(_vPos);
	}

	const _float ATransform::ComputeDistanceSq(const Vector3 _vPos)
	{
		return m_mWorldMatrix.ComputeDistanceSq(_vPos);
	}

	void ATransform::GravityFall(const _double& _deltaTime)
	{
		m_vVelocity += m_vGravity * static_cast<_float>(_deltaTime);
		Vector3 vPosition = GetPos();
		vPosition += m_vVelocity * static_cast<_float>(_deltaTime);
		SetPos(vPosition);
	}

	void ATransform::DisableGravity()
	{
		m_vVelocity = Vector3(0.0f, 0.0f, 0.0f);
	}

	void ATransform::DisableJump()
	{
		m_vJumpvelocity = Vector3(0.0f, 12.f, 0.0f);
	}

	void ATransform::JumpMovement(const _double& _deltaTime)
	{
		m_vJumpvelocity += m_vGravity * static_cast<_float>(_deltaTime);
		Vector3 vPosition = GetPos();
		vPosition += m_vJumpvelocity * static_cast<_float>(_deltaTime);
		SetPos(vPosition);
	}


	void ATransform::Free()
	{
	}

}