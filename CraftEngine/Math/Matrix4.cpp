#include "Matrix4.h"
#include <cstring>
#include <algorithm>

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

	Matrix4& Matrix4::operator=(const Matrix4 other)
	{
		memcpy(elements, other.elements, sizeof(elements));
		return *this;
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
}