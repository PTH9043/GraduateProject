#include "CoreDefines.h"
#include "UTransform.h"

namespace Core {

	UTransform::UTransform() : m_mWorldMatrix{_matrix::Identity}, m_Rotation{}, m_vOriginScale{}
	{
	}

	const Vector3 UTransform::GetAngles() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return Vector3(glm::degrees(vEuler.x), glm::degrees(vEuler.y), glm::degrees(vEuler.z));
	}

	const _float UTransform::GetXAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.x);
	}

	const _float UTransform::GetYAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.y);
	}

	const _float UTransform::GetZAngle() const
	{
		Vector3 vEuler = m_Rotation.ToEuler();
		return glm::degrees(vEuler.z);
	}

	void UTransform::RotateFix(const Vector3 _vStandardAngle, const _float _TurnAnge)
	{
		m_Rotation = _quaternion::CreateFromAxisAngle(_vStandardAngle, _TurnAnge);

		_matrix RotationMatrix = _matrix::CreateFromQuaternion(m_Rotation);

		Vector3 vScale = GetScale();

		SetRight(PTH::TransformNormal(Vector3{ vScale.x, 0.f, 0.f }, RotationMatrix));
		SetUp(PTH::TransformNormal(Vector3{ 0.f, vScale.y, 0.f }, RotationMatrix));
		SetLook(PTH::TransformNormal(Vector3{ 0.f, 0.f, vScale.z }, RotationMatrix));
	}

	void UTransform::RotateFix(const Vector3 _vAngle)
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

	void UTransform::RotateTurn(const Vector3 _vAxis, const _float _Angle)
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

	void UTransform::RotateTurn(const Vector3 _vAngle)
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

	void UTransform::LookAt(const Vector3 _vTargetPos)
	{
		m_mWorldMatrix.LookAt(_vTargetPos);
	}

	const _float UTransform::ComputeDistance(const Vector3 _vPos)
	{
		return m_mWorldMatrix.ComputeDistance(_vPos);
	}

	const _float UTransform::ComputeDistanceSq(const Vector3 _vPos)
	{
		return m_mWorldMatrix.ComputeDistanceSq(_vPos);
	}


	void UTransform::Free()
	{
	}

}