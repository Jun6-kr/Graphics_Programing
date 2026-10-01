#include "Matrix4.h"
#include "MathDefines.h"
#include <cstring>
#include <algorithm>
#include <cmath>

namespace Craft
{
	const Matrix4 Matrix4::Identity = Matrix4();

	Matrix4::Matrix4()
	{
		// 단위 행렬로 만들어서 생성
		// 1. 모든 성분을 0으로 설정
		memset(elements, 0, sizeof(elements));

		// 2. 대각 성분만 1로 설정
		m00 = 1.0f;
		m11 = 1.0f;
		m22 = 1.0f;
		m33 = 1.0f;
	}

	Matrix4::Matrix4(const Matrix4 & other)
	{
		memcpy(elements, other.elements, sizeof(elements));
	}

	Matrix4 Matrix4::Transpose(const Matrix4& matrix)
	{
		// 반환할 행렬 선언
		Matrix4 m = matrix;

		// 대각 성분을 기준으로 행과 열을 교환
		std::swap(m.m01, m.m10);
		std::swap(m.m02, m.m20);
		std::swap(m.m03, m.m30);
		std::swap(m.m21, m.m12);
		std::swap(m.m31, m.m13);
		std::swap(m.m32, m.m23);

		return m;
	}

	Matrix4 Matrix4::InverseRotation(const Matrix4& matrix)
	{
		return Transpose(matrix);
	}

	Matrix4 Matrix4::Translation(float x, float y, float z)
	{
		Matrix4 m;

		m.m00 = 1.0f;	m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
		m.m10 = 0.0f;	m.m11 = 1.0f;	m.m12 = 0.0f;	m.m13 = 0.0f;
		m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = 1.0f;	m.m23 = 0.0f;
		m.m30 = x;		m.m31 = y;		m.m32 = z;		m.m33 = 1.0f;

		return m;
	}

	Matrix4 Matrix4::Translation(const Vector3& translation)
	{
		return Translation(translation.x, translation.y, translation.z);
	}

	Matrix4 Matrix4::Scale(float x, float y, float z)
	{
		Matrix4 m;

		m.m00 = x;		m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
		m.m10 = 0.0f;	m.m11 = y;		m.m12 = 0.0f;	m.m13 = 0.0f;
		m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = z;		m.m23 = 0.0f;
		m.m30 = 0.0f;	m.m31 = 0.0f;	m.m32 = 0.0f;	m.m33 = 1.0f;

		return m;
	}

	Matrix4 Matrix4::Scale(const Vector3& scale)
	{
		return Scale(scale.x, scale.y, scale.z);
	}

	Matrix4 Matrix4::Scale(float scale)
	{
		return Scale(scale, scale, scale);
	}

	Matrix4 Matrix4::Rotation(float x, float y, float z)
	{
		// 무조건은 아니지만, 일반적으로 x -> y -> z축 순으로 회전을 적용
		return RotationX(x) * RotationY(y) * RotationZ(z);
	}

	Matrix4 Matrix4::Rotation(const Vector3& rotation)
	{
		return Rotation(rotation.x, rotation.y, rotation.z);
	}

	// X축 회전
	Matrix4 Matrix4::RotationX(float angle)
	{
		// [ C  S]
		// [-S  C]

		// 코사인/사인 값 구하기
		float cosAngle = std::cos(angle * DegreesToRadians);
		float sinAngle = std::sin(angle * DegreesToRadians);

		Matrix4 m;

		// X축은 회전에 영향을 받지 않는다
		m.m00 = 1.0f;	m.m01 = 0.0f;		m.m02 = 0.0f;		m.m03 = 0.0f;
		m.m10 = 0.0f;	m.m11 = cosAngle;	m.m12 = sinAngle;	m.m13 = 0.0f;
		m.m20 = 0.0f;	m.m21 = -sinAngle;	m.m22 = cosAngle;	m.m23 = 0.0f;
		m.m30 = 0.0f;	m.m31 = 0.0f;		m.m32 = 0.0f;		m.m33 = 1.0f;


		return m;
	}

	Matrix4 Matrix4::RotationY(float angle)
	{
		// [ C  S]
		// [-S  C]

		// 코사인/사인 값 구하기
		float cosAngle = std::cos(angle * DegreesToRadians);
		float sinAngle = std::sin(angle * DegreesToRadians);

		Matrix4 m;

		// Y축은 회전에 영향을 받지 않는다 - Y축 회전은 sin에 -1을 곱해준다
		// TO. Codex 이거 설명해줘.`
		m.m00 = cosAngle;	m.m01 = 0.0f;	m.m02 = -sinAngle;	m.m03 = 0.0f;
		m.m10 = 0.0f;		m.m11 = 1.0f;	m.m12 = 0.0f;		m.m13 = 0.0f;
		m.m20 = sinAngle;	m.m21 = 0.0f;	m.m22 = cosAngle;	m.m23 = 0.0f;
		m.m30 = 0.0f;		m.m31 = 0.0f;	m.m32 = 0.0f;		m.m33 = 1.0f;


		return m;
	}

	// Z축 회전
	Matrix4 Matrix4::RotationZ(float angle)
	{
		// [ C  S]
		// [-S  C]

		// 코사인/사인 값 구하기
		float cosAngle = std::cos(angle * DegreesToRadians);
		float sinAngle = std::sin(angle * DegreesToRadians);

		Matrix4 m;

		// Z축은 회전에 영향을 받지 않는다
		m.m00 = cosAngle;	m.m01 = sinAngle;	m.m02 = 0.0f;	m.m03 = 0.0f;
		m.m10 = -sinAngle;	m.m11 = cosAngle;	m.m12 = 0.0f;	m.m13 = 0.0f;
		m.m20 = 0.0f;		m.m21 = 0.0f;		m.m22 = 1.0f;	m.m23 = 0.0f;
		m.m30 = 0.0f;		m.m31 = 0.0f;		m.m32 = 0.0f;	m.m33 = 1.0f;


		return m;
	}

	Matrix4& Matrix4::operator=(const Matrix4 other)
	{
		memcpy(elements, other.elements, sizeof(elements));
		return *this;
	}

	Matrix4 Matrix4::operator*(const Matrix4 other) const
	{
		Matrix4 m;

		// 1행
		m.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
		m.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
		m.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
		m.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;

		// 2행
		m.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
		m.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
		m.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
		m.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;

		// 3행
		m.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
		m.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
		m.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
		m.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;

		// 4행
		m.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;
		m.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
		m.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
		m.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

		return m;
	}

	Matrix4& Matrix4::operator*=(const Matrix4 other)
	{
		*this = *this * other;
		return *this;
	}

	Vector3 operator*(const Vector3& vector, const Matrix4& matrix)
	{
		// 행벡터로 취급해서 계산
		// 1x4 행렬 형태로 취급 -> 4번째 행의 성분은 1로 처리
		// 4번째 행의 성분이 1이면 점(위치) / 0이면 벡터(방향)
		Vector3 result;
		result.x = vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20 + matrix.m30;
		result.y = vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21 + matrix.m31;
		result.z = vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22 + matrix.m32;


		return result;
	}
}