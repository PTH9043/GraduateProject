#include "PthMath.h"

using namespace PTH;


Matrix& PTH::Matrix::operator=(const matrix4x4& m)
{
	this->m = m;
	return *this;
}

PTH::Matrix::Matrix(matrix4x4&& m)
{
	this->m = m;
}

Matrix& PTH::Matrix::operator=(matrix4x4&& m)
{
	this->m = m;
	return *this;
}

Matrix& PTH::Matrix::operator=(const Matrix& m)
{
	this->m = m.m;
	return *this;
}

PTH::Matrix::Matrix(Matrix&& m)
{
	this->m = m.m;
}

Matrix& PTH::Matrix::operator=(Matrix&& m)
{
	this->m = m.m;
	return *this;
}


bool PTH::Matrix::operator==(const matrix4x4& M) const noexcept
{
	return m == M;
}

bool PTH::Matrix::operator!=(const matrix4x4& M) const noexcept
{
	return false;
}

Matrix& PTH::Matrix::operator+=(const matrix4x4& M) noexcept
{
	m = m + M;
	return *this;
}

Matrix& PTH::Matrix::operator-=(const matrix4x4& M) noexcept
{
	m = m - M;
	return *this;
}

Matrix& PTH::Matrix::operator*=(const matrix4x4& M) noexcept
{
	m = m * M;
	return *this;
}

Matrix& PTH::Matrix::operator*=(float S) noexcept
{
	m = m * S;
	return *this;
}

Matrix& PTH::Matrix::operator/=(float S) noexcept
{
	m = m  / S;
	return *this;
}

Matrix& PTH::Matrix::operator/=(const matrix4x4& M) noexcept
{
	m = m / M;
	return *this;
}

Vector3 PTH::Matrix::GetScale() const
{
	Vector3 vRight = GetRight();
	Vector3 vUp = GetUp();
	Vector3 vLook = GetLook();
	return Vector3{ glm::length(vRight) , glm::length(vUp), glm::length(vLook) };
}

void PTH::Matrix::SetScale(Vector3 _vScale) const
{
	Vector3 vRight = GetRight();
	Vector3 vUp = GetUp();
	Vector3 vLook = GetLook();

	SetRight(glm::normalize(vRight) * _vScale.x);
	SetUp(glm::normalize(vUp) * _vScale.y);
	SetLook(glm::normalize(vLook) * _vScale.z);
}

void PTH::Matrix::Decompose(OUT Vector3& scale, OUT Quaternion& rotation, OUT  Vector3& translation) noexcept
{
	translation = glm::vec3(m[3]);
	scale = glm::vec3(
		glm::length(glm::vec3(m[0])),
		glm::length(glm::vec3(m[1])),
		glm::length(glm::vec3(m[2]))
	);

	// 회전 행렬 추출
	glm::mat3 rotationMatrix(
		glm::normalize(glm::vec3(m[0])),
		glm::normalize(glm::vec3(m[1])),
		glm::normalize(glm::vec3(m[2]))
	);

	// 추출된 회전 행렬을 쿼터니언으로 변환
	rotation = glm::quat_cast(rotationMatrix);
}

Matrix PTH::Matrix::Transpose() const noexcept
{
	return glm::transpose(m);
}

void PTH::Matrix::Transpose(OUT Matrix& result) const noexcept
{
	result = glm::transpose(m);
}

Matrix PTH::Matrix::Invert() const noexcept
{
	return glm::inverse(m);
}

void PTH::Matrix::Invert(OUT Matrix& result) const noexcept
{
	result = glm::inverse(m);
}

float PTH::Matrix::Determinant() const noexcept
{
	return glm::determinant(m);
}

Vector3 PTH::Matrix::ToEuler() const noexcept
{
	Vector3 v;
	glm::extractEulerAngleXYZ(m, v.x, v.y, v.z);
	return v;
}

Matrix PTH::Matrix::CreateBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, const Vector3* cameraForward) noexcept
{
	// 오브젝트에서 카메라를 향하는 방향 벡터 계산
	glm::vec3 forward;
	if (cameraForward) {
		forward = glm::normalize(*cameraForward);
	}
	else {
		forward = glm::normalize(cameraPosition - object);
	}

	// 오브젝트의 오른쪽 방향 벡터 계산 (cross product 사용)
	glm::vec3 right = glm::normalize(glm::cross(cameraUp, forward));

	// 오브젝트의 위쪽 방향 벡터 계산
	glm::vec3 up = glm::cross(forward, right);

	// Billboard 행렬 생성
	glm::mat4 billboardMatrix = glm::mat4(
		right.x, up.x, -forward.x, 0.0f,
		right.y, up.y, -forward.y, 0.0f,
		right.z, up.z, -forward.z, 0.0f,
		object.x, object.y, object.z, 1.0f
	);

	return billboardMatrix;
}

Matrix PTH::Matrix::CreateConstrainedBillboard(const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis, const Vector3* cameraForward, const Vector3* objectForward) noexcept
{
	// 오브젝트에서 카메라를 향하는 방향 벡터 계산
	glm::vec3 forward;
	if (cameraForward) {
		forward = glm::normalize(*cameraForward);
	}
	else {
		forward = glm::normalize(cameraPosition - object);
	}

	// 오브젝트의 오른쪽 방향 벡터 계산 (cross product 사용)
	glm::vec3 right = glm::normalize(glm::cross(rotateAxis, forward));

	// 오브젝트의 위쪽 방향 벡터 계산
	glm::vec3 up = glm::cross(forward, right);

	// Constrained Billboard 행렬 생성
	glm::mat4 constrainedBillboardMatrix = glm::mat4(
		right.x, up.x, -forward.x, 0.0f,
		right.y, up.y, -forward.y, 0.0f,
		right.z, up.z, -forward.z, 0.0f,
		object.x, object.y, object.z, 1.0f
	);

	return constrainedBillboardMatrix;
}

Matrix PTH::Matrix::CreateTranslation(const Vector3& position) noexcept
{
	return  glm::translate(Identity, position);
}

Matrix PTH::Matrix::CreateTranslation(const float& x, const float& y, const float& z) noexcept
{
	return  glm::translate(Identity, Vector3(x, y, z));
}

Matrix PTH::Matrix::CreateScale(const Vector3& scales) noexcept
{
	return  glm::scale(Identity, scales);
}

Matrix PTH::Matrix::CreateScale(const float& xs, const float& ys, const float& zs) noexcept
{
	return  glm::scale(Identity, Vector3(xs, ys, zs));
}

Matrix PTH::Matrix::CreateScale(const float& scale) noexcept
{
	return  glm::scale(Identity, Vector3(scale, scale, scale));
}

Matrix PTH::Matrix::CreateRotationX(const float& radians) noexcept
{
	return glm::rotate(glm::mat4(1.0f), radians, glm::vec3(1.0f, 0.0f, 0.0f));
}

Matrix PTH::Matrix::CreateRotationY(const float& radians) noexcept
{
	return glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));
}

Matrix PTH::Matrix::CreateRotationZ(const float& radians) noexcept
{
	return glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 0.0f, 1.0f));
}

Matrix PTH::Matrix::CreateFromAxisAngle(const Vector3& axis, const float& angle) noexcept
{
	return glm::rotate(glm::mat4(1.0f), angle, axis);
}

Matrix PTH::Matrix::CreatePerspectiveOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& nearPlane, const float& farPlane) noexcept
{
	return glm::frustumLH(left, right, bottom, top, nearPlane, farPlane);
}

Matrix PTH::Matrix::CreateOrthographicLH(const float& width, const float& height, const float& zNearPlane, const float& zFarPlane) noexcept
{
	return glm::orthoLH(0.0f, width, 0.0f, height, zNearPlane, zFarPlane);
}

Matrix PTH::Matrix::CreateOrthographicOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& zNearPlane, const float& zFarPlane) noexcept
{
	return glm::orthoLH(left, right, bottom, top, zNearPlane, zFarPlane);
}

Matrix PTH::Matrix::CreatePerspectiveForLH(const float& fov, const float& width, const float& height, const float& Near, const float& Far)
{
	return glm::perspectiveLH(fov, width / height, Near, Far);
}

Matrix PTH::Matrix::CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept
{
	return glm::lookAt(position, target, up);
}

Matrix PTH::Matrix::CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept
{
	glm::vec3 right = glm::cross(up, forward);
	return glm::mat4(glm::vec4(right, 0.0f),
		glm::vec4(up, 0.0f),
		glm::vec4(forward, 0.0f),
		glm::vec4(position, 1.0f));
}

Matrix PTH::Matrix::CreateFromQuaternion(const Quaternion& quat) noexcept
{
	return glm::toMat4(quat.GetQuaternion());
}

Matrix PTH::Matrix::CreateFromYawPitchRoll(const float& yaw, const float& pitch, const float& roll) noexcept
{
	return glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));
}

Matrix PTH::Matrix::CreateFromYawPitchRoll(const Vector3& angles) noexcept
{
	return glm::rotate(glm::mat4(1.0f), angles.y, glm::vec3(0.0f, 1.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), angles.x, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), angles.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void PTH::Matrix::Lerp(const Matrix& M1, const Matrix& M2, const float& t, Matrix& result) noexcept
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = glm::lerp(M1.m[i][j], M2.m[i][j], t);
		}
	}
}

Matrix PTH::Matrix::Lerp(const Matrix& M1, const Matrix& M2, const float& t) noexcept
{
	Matrix mat;
	Lerp(M1, M2, t, mat);
	return mat;
}

void PTH::Matrix::Transform(const Matrix& M, const Quaternion& rotation, Matrix& result) noexcept
{
	result = M * glm::toMat4(rotation.GetQuaternion());
}

Matrix PTH::Matrix::Transform(const Matrix& M, const Quaternion& rotation) noexcept
{
	return M * glm::toMat4(rotation.GetQuaternion());
}

const Matrix PTH::Matrix::MatrixSetScaling(const Vector3& _v3Scaling) noexcept
{
	return glm::scale(m, _v3Scaling);
}

const Matrix PTH::Matrix::MatrixSetRotationFix(const Vector3& _vRotation) noexcept
{
	Vector3 vScale = GetScaling();

	// Euler 회전값 (x, y, z) 설정
	glm::vec3 eulerRotation(_vRotation.x, _vRotation.y, _vRotation.z);
	// Euler 회전값으로부터 쿼터니언 생성
	glm::quat quaternionRotation = glm::quat(eulerRotation);

	Matrix mat = glm::scale(glm::toMat4(quaternionRotation), vScale);
	mat.SetPos(GetPos());
	// 쿼터니언 행렬로 변환
	return  mat;
}

const Matrix PTH::Matrix::MatrixSetRotationTurn(const Vector3& _vRotation) noexcept
{
	Vector3 vScale = GetScaling();

	// 기존 행렬을 쿼터니언으로 변환
	glm::quat originalQuaternion = glm::quat_cast(m);

	// 새로운 회전값을 적용하여 쿼터니언을 업데이트
	glm::quat newQuaternion = glm::rotate(originalQuaternion, _vRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	newQuaternion = glm::rotate(newQuaternion, _vRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	newQuaternion = glm::rotate(newQuaternion, _vRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	Matrix mat = glm::scale(glm::toMat4(newQuaternion), vScale);
	mat.SetPos(GetPos());
	// 쿼터니언 행렬로 변환
	return  mat;
}

const Vector3 PTH::Matrix::GetScaling() noexcept
{
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3(m[0])); // 첫 번째 열이 x 축의 스케일
	scale.y = glm::length(glm::vec3(m[1])); // 두 번째 열이 y 축의 스케일
	scale.z = glm::length(glm::vec3(m[2])); // 세 번째 열이 z 축의 스케일
	return scale;
}

const void PTH::Matrix::Combine(const Vector3& _vScale, const Vector3& _vPos, const Quaternion& _vRotation) noexcept
{
	matrix4x4 scaleMatrix = glm::scale(Identity, _vScale);
	matrix4x4 rotationMatrix = glm::toMat4(_vRotation.GetQuaternion());
	matrix4x4 translationMatrix = glm::translate(Identity, _vPos);

	// 스케일, 회전, 위치 순서로 행렬을 조합
	this->m = translationMatrix * rotationMatrix * scaleMatrix;
}

void PTH::Matrix::LookAt(const Vector3& _vPos)
{
	// Pos Look
	Vector3 vPos = GetPos();
	Vector3 Look = glm::normalize(vPos - _vPos);
	
	Vector3 Right = glm::normalize(glm::cross(UpVec3, Look));
	Vector3 Up = glm::normalize(glm::cross(Look, Right));
	glm::qua q = glm::quatLookAt(Look, Up);
	Matrix mat = glm::toMat4(q);

	mat.SetRight(glm::vec3(mat.m * glm::vec4(GetRight(), 0.f)));
	mat.SetUp(glm::vec3(mat.m * glm::vec4(GetUp(), 0.f)));
	mat.SetLook(glm::vec3(mat.m * glm::vec4(GetLook(), 0.f)));
}

void PTH::Matrix::MoveForward(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	Vector3 vLook = GetLook();

	vPos += vLook * static_cast<float>(_dTimeDelta * _fSpeed);
	SetPos(vPos);
}

void PTH::Matrix::MoveBack(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	Vector3 vLook = GetLook();

	vPos -= vLook * static_cast<float>(_dTimeDelta * _fSpeed);
	SetPos(vPos);
}

void PTH::Matrix::MoveLeft(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	Vector3 vRight = GetRight();

	vPos -= vRight * static_cast<float>(_dTimeDelta * _fSpeed);
	SetPos(vPos);
}

void PTH::Matrix::MoveRight(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	Vector3 vRight = GetRight();

	vPos += vRight * static_cast<float>(_dTimeDelta * _fSpeed);
	SetPos(vPos);
}

void PTH::Matrix::MoveForwardNotY(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;
	Vector3 vLook = GetLook();

	vPos += vLook * static_cast<float>(_dTimeDelta * _fSpeed);
	vPos.y = yPos;
	SetPos(vPos);
}

void PTH::Matrix::MoveBackNotY(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;
	Vector3 vLook = GetLook();

	vPos -= vLook * static_cast<float>(_dTimeDelta * _fSpeed);
	vPos.y = yPos;
	SetPos(vPos);
}

void PTH::Matrix::MoveLeftNotY(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;
	Vector3 vRight = GetRight();

	vPos -= vRight * static_cast<float>(_dTimeDelta * _fSpeed);
	vPos.y = yPos;
	SetPos(vPos);
}

void PTH::Matrix::MoveRightNotY(const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;
	Vector3 vRight = GetRight();

	vPos += vRight * static_cast<float>(_dTimeDelta * _fSpeed);
	vPos.y = yPos;
	SetPos(vPos);
}

void PTH::Matrix::TranslatePos(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed, const float& _fLimitDistance)
{
	Vector3 vPos = GetPos();

	if (ComputeDistance(_vPos) >= _fLimitDistance)
	{
		Vector3 vLook = _vPos - vPos;
		vPos += glm::normalize(vLook) * static_cast<float>(_dTimeDelta * _fSpeed);
		SetPos(vPos);
	}
}

void PTH::Matrix::TranslateDir(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();

	vPos += _vDir * static_cast<float>(_dTimeDelta * _fSpeed);
	SetPos(vPos);
}

void PTH::Matrix::TranslatePosNotY(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed, const float& _fLimitDistance)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;
	Vector3 vLook = _vPos - vPos;

	if (ComputeDistance(vLook) >= _fLimitDistance)
	{
		vPos += glm::normalize(vLook) * static_cast<float>(_dTimeDelta * _fSpeed);
		vPos.y = yPos;
		SetPos(vPos);
	}
}

void PTH::Matrix::TranslateDirNotY(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed)
{
	Vector3 vPos = GetPos();
	float yPos = vPos.y;

	vPos += _vDir * static_cast<float>(_dTimeDelta * _fSpeed);
	vPos.y = yPos;
	SetPos(vPos);
}

const float PTH::Matrix::ComputeDistance(const Vector3& _vPos)
{
	return glm::distance(GetPos(), _vPos);
}

const float PTH::Matrix::ComputeDistanceSq(const Vector3& _vPos)
{
	Vector3 v = GetPos() - _vPos;
	return v.x * v.x + v.y * v.y  + v.z * v.z;
}

Matrix PTH::operator+(const Matrix& M1, const Matrix& M2) noexcept
{
	return M1.m + M2.m;
}

Matrix PTH::operator-(const Matrix& M1, const Matrix& M2) noexcept
{
	return M1.m - M2.m;
}

Matrix PTH::operator*(const Matrix& M1, const Matrix& M2) noexcept
{
	return M1.m * M2.m;
}

Matrix PTH::operator*(const Matrix& M, float S) noexcept
{
	return M.m * S;
}

Matrix PTH::operator/(const Matrix& M, float S) noexcept
{
	return M.m / S;
}

Matrix PTH::operator/(const Matrix& M1, const Matrix& M2) noexcept
{
	return M1.m / M2.m;
}

Matrix PTH::operator*(float S, const Matrix& M) noexcept
{
	return M.m * S;
}

PTH::Quaternion::Quaternion(const Quaternion& _quat) : quat{_quat.quat}
{
}

Quaternion& PTH::Quaternion::operator=(const Quaternion& _quat) 
{
	quat = _quat.quat;
	return *this;
}

PTH::Quaternion::Quaternion(Quaternion&& _quat) : quat{ _quat.quat }
{
}

Quaternion& PTH::Quaternion::operator=(Quaternion&& _quat)
{
	quat = _quat.quat;
	return *this;
}

PTH::Quaternion::Quaternion(const glm::quat& _q)
{
	::memcpy(&quat, &_q, sizeof(glm::vec4));
}

Quaternion& PTH::Quaternion::operator=(const glm::quat& _q)
{
	::memcpy(&quat, &_q, sizeof(glm::vec4));
	return *this;
}

PTH::Quaternion::Quaternion(glm::quat&& _q)
{
	::memcpy(&quat, &_q, sizeof(glm::vec4));
}

Quaternion& PTH::Quaternion::operator=(glm::quat&& _q)
{
	::memcpy(&quat, &_q, sizeof(glm::vec4));
	return *this;
}

bool PTH::Quaternion::operator==(const Quaternion& q) const noexcept
{
	return quat == q.quat;
}

bool PTH::Quaternion::operator!=(const Quaternion& q) const noexcept
{
	return quat != q.quat;
}

Quaternion& PTH::Quaternion::operator+=(const Quaternion& q) noexcept
{
	quat += q.quat;
	return *this;
}

Quaternion& PTH::Quaternion::operator-=(const Quaternion& q) noexcept
{
	quat -= q.quat;
	return *this;
}

Quaternion& PTH::Quaternion::operator*=(const Quaternion& q) noexcept
{
	quat *= q.quat;
	return *this;
}

Quaternion& PTH::Quaternion::operator*=(float S) noexcept
{
	quat *= S;
	return *this;
}

Quaternion& PTH::Quaternion::operator/=(const Quaternion& q) noexcept
{
	quat /= q.quat;
	return *this;
}

Quaternion PTH::Quaternion::operator-() const noexcept
{
	quat *= -1.f;
	return Quaternion();
}

glm::quat PTH::Quaternion::GetQuaternion() const
{
	glm::quat q;
	::memcpy(&q, &quat, sizeof(glm::quat));
	return q;
}

float PTH::Quaternion::Length() const noexcept
{
	return glm::length(quat);
}

float PTH::Quaternion::LengthSquared() const noexcept
{
	return glm::length2(quat);
}

void PTH::Quaternion::Normalize() noexcept
{
	quat = glm::normalize(quat);
}

void PTH::Quaternion::Normalize(Quaternion& result) const noexcept
{
	result.quat = glm::normalize(quat);
}

void PTH::Quaternion::Conjugate() noexcept
{
	glm::quat quatRepresentation(quat.w, quat.x, quat.y, quat.z);
	glm::quat conjugateQuat = glm::conjugate(quatRepresentation);
	::memcpy(&quat, &conjugateQuat, sizeof(glm::vec4));
}

void PTH::Quaternion::Conjugate(Quaternion& result) const noexcept
{
	glm::quat quatRepresentation(quat.w, quat.x, quat.y, quat.z);
	glm::quat conjugateQuat = glm::conjugate(quatRepresentation);
	result.quat = Vector4{ conjugateQuat.x, conjugateQuat.y, conjugateQuat.z, conjugateQuat.w };
}

float PTH::Quaternion::Dot(const Quaternion& Q) const noexcept
{
	return glm::dot(quat, Q.quat);
}

Vector3 PTH::Quaternion::ToEuler() const noexcept
{
	const float xx = quat.x * quat.x;
	const float yy = quat.y * quat.y;
	const float zz = quat.z * quat.z;

	const float m31 = 2.f * quat.x * quat.z + 2.f * quat.y * quat.w;
	const float m32 = 2.f * quat.y * quat.z - 2.f * quat.x * quat.w;
	const float m33 = 1.f - 2.f * xx - 2.f * yy;

	const float cy = sqrtf(m33 * m33 + m31 * m31);
	const float cx = atan2f(-m32, cy);
	if (cy > 16.f * FLT_EPSILON)
	{
		const float m12 = 2.f * quat.x * quat.y + 2.f * quat.z * quat.w;
		const float m22 = 1.f - 2.f * xx - 2.f * zz;

		return Vector3(cx, atan2f(m31, m33), atan2f(m12, m22));
	}
	else
	{
		const float m11 = 1.f - 2.f * yy - 2.f * zz;
		const float m21 = 2.f * quat.x * quat.y - 2.f * quat.z * quat.w;

		return Vector3(cx, 0.f, atan2f(-m21, m11));
	}
}

Quaternion PTH::Quaternion::CreateFromAxisAngle(const Vector3& axis, const float& angle) noexcept
{
	glm::quat R = glm::angleAxis(angle, axis);
	Quaternion resultVec4(R.x, R.y, R.z, R.w);
	return resultVec4;
}

Quaternion PTH::Quaternion::CreateFromYawPitchRoll(const float& yaw, const float& pitch, const float& roll) noexcept
{
	glm::quat R = glm::yawPitchRoll(yaw, pitch, roll);
	Quaternion resultVec4(R.x, R.y, R.z, R.w);
	return resultVec4;
}

Quaternion PTH::Quaternion::CreateFromYawPitchRoll(const Vector3& angles) noexcept
{
	glm::quat R = glm::yawPitchRoll(angles.x, angles.y, angles.z);
	Quaternion resultVec4(R.x, R.y, R.z, R.w);
	return resultVec4;
}

Quaternion PTH::Quaternion::CreateFromRotationMatrix(const Matrix& M) noexcept
{
	glm::quat R = glm::quat_cast(M.m);
	Quaternion resultVec4(R.x, R.y, R.z, R.w);
	return resultVec4;
}

void PTH::Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float& t, Quaternion& result) noexcept
{
	glm::vec4 Q0 = q1.quat;
	glm::vec4 Q1 = q2.quat;

	// glm::dot 함수로 대체
	float dot = glm::dot(Q0, Q1);

	glm::vec4 R;
	if (dot >= 0.0f) {
		// glm::lerp 함수로 대체
		R = glm::lerp(Q0, Q1, t);
	}
	else {
		// glm::replicate 함수는 glm에서 직접 사용할 수 없습니다.
		// glm::vec4(tv)로 대체
		float tv = t;
		float t1v = 1.0f - t;
		glm::vec4 X0 = Q0 * t1v;
		glm::vec4 X1 = Q1 * tv;
		R = X0 - X1;
	}

	// glm::normalize 함수로 대체
	result.quat = glm::normalize(R);
}

Quaternion PTH::Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float& t) noexcept
{
	Quaternion quat;
	Lerp(q1, q2, t, quat);
	return quat;
}

void PTH::Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float& t, Quaternion& result) noexcept
{
	glm::quat  quat1 = q1.GetQuaternion();
	glm::quat quat2 = q2.GetQuaternion();
	result = glm::slerp(quat1, quat2, t);
}

Quaternion PTH::Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float& t) noexcept
{
	Quaternion quat;
	Slerp(q1, q2, t, quat);
	return quat;
}

void PTH::Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept
{
	glm::quat  quat1 = q1.GetQuaternion();
	glm::quat quat2 = q2.GetQuaternion();
	result = glm::cross(quat1, quat2);
}

Quaternion PTH::Quaternion::Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept
{
	Quaternion quat;
	Concatenate(q1, q2, quat);
	return quat;
}

void __cdecl PTH::Quaternion::FromToRotation(const Vector3& fromDir, const Vector3& toDir, Quaternion& result) noexcept
{ 
	// glm::quat으로 변환
	result = glm::rotation(glm::normalize(fromDir), glm::normalize(toDir));
}

Quaternion PTH::Quaternion::FromToRotation(const Vector3& fromDir, const Vector3& toDir) noexcept
{
	Quaternion quat;
	FromToRotation(fromDir, toDir, quat);
	return quat;
}

void __cdecl PTH::Quaternion::LookRotation(const Vector3& forward, const Vector3& up, Quaternion& result) noexcept
{
	// glm::quat으로 변환
	result = glm::quat_cast(glm::lookAt(glm::vec3(0.0f), glm::normalize(forward), glm::normalize(up)));
}

Quaternion PTH::Quaternion::LookRotation(const Vector3& forward, const Vector3& up) noexcept
{
	Quaternion quat;
	LookRotation(forward, up, quat);
	return quat;
}

float PTH::Quaternion::Angle(const Quaternion& q1, const Quaternion& q2) noexcept
{
	glm::quat R = glm::cross(q1.GetQuaternion(), glm::conjugate(q2.GetQuaternion()));
	Quaternion q{ R };

	const float rs = R.w;
	return 2.0f * atan2(glm::length(glm::vec3(q.quat)), rs);
}

Vector4 PTH::PlaneFromPoints(const Vector3& _vPoint1, const Vector3& _vPoint2, const Vector3& _vPoint3)
{
	Vector3 v21 = _vPoint1 - _vPoint2;
	Vector3 v31 = _vPoint1 - _vPoint3;

	Vector3 N = glm::cross(v21, v31);
	N = glm::normalize(N);

	float D = glm::dot(N, _vPoint1) * -1.f;
	return Vector4(N, D);
}

float PTH::PlaneDotCoord(const Vector4& _Plane, const Vector3& _vPoint)
{
	Vector4 v3 = glm::mix(VectorOne, Vector4(_vPoint, 1.f), VectorSelect1110);
	return glm::dot(_Plane, v3);
}

Vector3 PTH::TransformNormal(const Vector3& normal, const Matrix& transform)
{
	// glm::mat3를 사용하여 벡터를 변환
	glm::mat3 rotationMat(transform.m);
	glm::vec3 transformedNormal = rotationMat * normal;

	// 정규화
	return glm::normalize(transformedNormal);
}

Vector3 PTH::TransformCoord(const Vector3& position, const Matrix& transform)
{
	glm::vec4 homogenousPos = transform.m * glm::vec4(position, 1.0f);
	// 화면 좌표계로 변환
	homogenousPos /= homogenousPos.w;
	return glm::vec3(homogenousPos);
}
