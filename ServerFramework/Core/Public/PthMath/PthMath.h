#ifndef _PTHMATH_PTHMATH_H
#define _PTHMATH_PTHMATH_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtx/vector_angle.hpp"
#include "gtx/rotate_vector.hpp"
#include "gtx/vector_query.hpp"
#include "gtx/euler_angles.hpp"
#include "gtx/associated_min_max.hpp"
#include "gtc/epsilon.hpp"
#include "gtc/bitfield.hpp"
#include "gtc/constants.hpp"
#include "gtx/dual_quaternion.hpp"
#include "gtc/quaternion.hpp"

namespace PTH
{
#define OUT 
#define IN
#define MOVE

	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
    using Quaternion = glm::quat;

	using matrix2x2 = glm::mat2x2;
	using matrix3x3 = glm::mat3x3;
	using matrix4x4 = glm::mat4;

	struct Matrix;
	struct OUTMATRIX;

	// OutMatrix
#pragma region OUTMATRIX
	struct OUTMATRIX
	{
		Vector3 vPos;
		Vector3 vScale;
		Vector3 vRot;
		Vector4 vQuaternionRot;
	};
#pragma endregion OUTMATRIX

    enum MATRIXROW : unsigned int
    {
        MATROW_RIGHT, MATROW_UP, MATROW_LOOK, MATROW_POS, MATROW_END
    };

    static constexpr Vector3 RightVec3{ 1.f, 0.f, 0.f };
    static constexpr Vector3 UpVec3{ 0.f, 1.f, 0.f };
    static constexpr Vector3 LookVec3{ 0.f, 0.f, 1.f };
    static constexpr Vector4 VectorOne{ 1.f, 1.f, 1.f, 1.f };
    static constexpr Vector4 VectorSelect1110{ 0xFFFFFFFF,0xFFFFFFFF, 0xFFFFFFFF, 0.f };

    //------------------------------------------------------------------------------
    //  4x4 Matrix (assumes right-handed cooordinates)
    struct Matrix
    {
        Matrix() noexcept
            : m(1.f, 0, 0, 0,
                0, 1.f, 0, 0,
                0, 0, 1.f, 0,
                0, 0, 0, 1.f)
        {
        }
        constexpr Matrix(const float& m00, const float& m01, const float& m02, const float& m03,
            const float& m10, const float& m11, const float& m12, const float& m13,
            const float& m20, const float& m21, const float& m22, const float& m23,
            const float& m30, const float& m31, const float& m32, const float& m33) noexcept
            : m(m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33)
        {
        }
        explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) noexcept
            : m(r0.x, r0.y, r0.z, 0,
                r1.x, r1.y, r1.z, 0,
                r2.x, r2.y, r2.z, 0,
                0, 0, 0, 1.f)
        {
        }
        explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) noexcept
            : m(r0.x, r0.y, r0.z, r0.w,
                r1.x, r1.y, r1.z, r1.w,
                r2.x, r2.y, r2.z, r2.w,
                r3.x, r3.y, r3.z, r3.w)
        {
        }
        Matrix(const matrix4x4& M) noexcept { memcpy(this, &M, sizeof(matrix4x4)); }
        Matrix& operator=(const matrix4x4& m) ;
        Matrix(matrix4x4&& m) ;
        Matrix& operator=(matrix4x4&& m) ;

        Matrix(const Matrix& M) noexcept { memcpy(this, &M.m, sizeof(Matrix)); }
        Matrix& operator=(const Matrix& m);
        Matrix(Matrix&& m);
        Matrix& operator=(Matrix&& m);

        // Comparison operators
        bool operator == (const matrix4x4& M) const noexcept;
        bool operator != (const matrix4x4& M) const noexcept;

        // Assignment operators
        Matrix& operator+= (const matrix4x4& M) noexcept;
        Matrix& operator-= (const matrix4x4& M) noexcept;
        Matrix& operator*= (const matrix4x4& M) noexcept;
        Matrix& operator*= (float S) noexcept;
        Matrix& operator/= (float S) noexcept;


       friend Matrix operator+ (const Matrix& M1, const Matrix& M2) noexcept;
       friend  Matrix operator- (const Matrix& M1, const Matrix& M2) noexcept;
       friend Matrix operator* (const Matrix& M1, const Matrix& M2) noexcept;
       friend  Matrix operator* (const Matrix& M, float S) noexcept;
       friend Matrix operator/ (const Matrix& M, float S) noexcept;
       friend Matrix operator/ (const Matrix& M1, const Matrix& M2) noexcept;
        // Element-wise divide
       friend Matrix operator* (float S, const Matrix& M) noexcept;

        Matrix& operator/= (const matrix4x4& M) noexcept;
        // Element-wise divide

    // Unary operators
        Matrix operator+ () const noexcept { return m; }
        Matrix operator- () const noexcept { return m * -1.f; }


        const Vector3& GetRight() { return *((glm::vec3*)(&m[MATROW_RIGHT][0])); }

        const Vector3& GetLeft() { return -1.f * GetRight(); }
        const Vector3& GetUp() { return *((glm::vec3*)(&m[MATROW_UP][0])); }
        const Vector3& GetDown() { return -1.f * GetUp(); }
        const Vector3& GetLook() { return *((glm::vec3*)(&m[MATROW_LOOK][0])); }
        const Vector3& GetBack() { return -1.f * GetLook(); }
        const Vector3& GetPos() { return *((glm::vec3*)(&m[MATROW_POS][0]));  }

        void SetRight(const Vector3& _vState) { *((glm::vec3*)(&m[MATROW_RIGHT][0])) = _vState; }
        void SetUp(const Vector3& _vState) { *((glm::vec3*)(&m[MATROW_UP][0])) = _vState; }
        void SetLook(const Vector3& _vState) { *((glm::vec3*)(&m[MATROW_LOOK][0])) = _vState; }
        void SetPos(const Vector3& _vState) { *((glm::vec3*)(&m[MATROW_POS][0])) = _vState; }

        // Matrix operations
        void Decompose(OUT Vector3& scale, OUT Quaternion& rotation, OUT  Vector3& translation) noexcept;

        Matrix Transpose() const noexcept;
        void Transpose(OUT Matrix& result) const noexcept;

        Matrix Invert() const noexcept;
        void Invert(OUT Matrix& result) const noexcept;

        float Determinant() const noexcept;

        // Computes rotation about y-axis (y), then x-axis (x), then z-axis (z)
        Vector3 ToEuler() const noexcept;

        // Static functions
        static Matrix CreateBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr) noexcept;

        static Matrix CreateConstrainedBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
            _In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr) noexcept;

        static Matrix CreateTranslation(const Vector3& position) noexcept;
        static Matrix CreateTranslation(const float& x, const float& y, const float& z) noexcept;

        static Matrix CreateScale(const Vector3& scales) noexcept;
        static Matrix CreateScale(const float& xs, const float& ys, const float& zs) noexcept;
        static Matrix CreateScale(const float& scale) noexcept;

        static Matrix CreateRotationX(const float& radians) noexcept;
        static Matrix CreateRotationY(const float& radians) noexcept;
        static Matrix CreateRotationZ(const float& radians) noexcept;

        static Matrix CreateFromAxisAngle(const Vector3& axis, const float& angle) noexcept;

        static Matrix CreatePerspectiveOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& nearPlane, const float& farPlane) noexcept;
        static Matrix CreateOrthographicLH(const float& width, const float& height, const float& zNearPlane, const float& zFarPlane) noexcept;
        static Matrix CreateOrthographicOffCenterLH(const float& left, const float& right, const float& bottom, const float& top, const float& zNearPlane, const float& zFarPlane) noexcept;
        static Matrix CreatePerspectiveForLH(const float& fov, const float& width, const float& height, const float& Near, const float& Far);

        static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
        static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;

        static Matrix CreateFromQuaternion(const Quaternion& quat) noexcept;

        // Rotates about y-axis (yaw), then x-axis (pitch), then z-axis (roll)
        static Matrix CreateFromYawPitchRoll(const float& yaw, const float& pitch, const float& roll) noexcept;

        // Rotates about y-axis (angles.y), then x-axis (angles.x), then z-axis (angles.z)
        static Matrix CreateFromYawPitchRoll(const Vector3& angles) noexcept;

        static void Lerp(const Matrix& M1, const Matrix& M2, const float& t, Matrix& result) noexcept;
        static Matrix Lerp(const Matrix& M1, const Matrix& M2, const float& t) noexcept;

        static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result) noexcept;
        static Matrix Transform(const Matrix& M, const Quaternion& rotation) noexcept;

        const  Matrix MatrixSetScaling(const Vector3& _v3Scaling) noexcept;
        const  Matrix MatrixSetRotationFix(const Vector3& _vRotation)noexcept;
        const Matrix MatrixSetRotationTurn(const Vector3& _vRotation)noexcept;
        const Vector3 GetScaling()noexcept;
        const void Combine(const Vector3& _vScale, const Vector3& _vPos, const Quaternion& _vRotation)noexcept;

        void LookAt(const Vector3& _vPos);

        // Move 
        void MoveForward(const double& _dTimeDelta, const float& _fSpeed);
        void MoveBack(const double& _dTimeDelta, const float& _fSpeed);
        void MoveLeft(const double& _dTimeDelta, const float& _fSpeed);
        void MoveRight(const double& _dTimeDelta, const float& _fSpeed);
        // Move Not Y
        void MoveForwardNotY(const double& _dTimeDelta, const float& _fSpeed);
        void MoveBackNotY(const double& _dTimeDelta, const float& _fSpeed);
        void MoveLeftNotY(const double& _dTimeDelta, const float& _fSpeed);
        void MoveRightNotY(const double& _dTimeDelta, const float& _fSpeed);
        // Translate To Pos
        void TranslatePos(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed,
            const float& _fLimitDistance = 0.1f);
        void TranslateDir(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed);
        // Translate To Pos Not Y
        void TranslatePosNotY(const Vector3& _vPos, const double& _dTimeDelta, const float& _fSpeed,
            const float& _fLimitDistance = 0.1f);
        void TranslateDirNotY(const Vector3& _vDir, const double& _dTimeDelta, const float& _fSpeed);
        // Compute Distance
        const float ComputeDistance(const Vector3& _vPos);
        const float ComputeDistanceSq(const Vector3& _vPos);

        // Constants
        static constexpr matrix4x4 Identity = 
            matrix4x4(1.f, 0, 0, 0,
                0, 1.f, 0, 0,
                0, 0, 1.f, 0,
                0, 0, 0, 1.f);

        private:
            mutable matrix4x4   m;
    };

    // Binary operators
    Matrix operator+ (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator- (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M1, const Matrix& M2) noexcept;
    // Element-wise divide
    Matrix operator* (float S, const Matrix& M) noexcept;

    
    Vector4 PlaneFromPoints(const Vector3& _vPoint1, const Vector3& _vPoint2,
        const Vector3& _vPoint3);

    float PlaneDotCoord(const Vector4& _Plane, const Vector3& _vPoint);
}

#endif // _PTHMATH_PTHMATH_H