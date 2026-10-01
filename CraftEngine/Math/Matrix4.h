#pragma once

#include <Math/Vector3.h>

namespace Craft
{
	// 변환(Transform) 함수를 제공하는 4x4 행렬
	class Matrix4
	{
	public:
		Matrix4();
		// 복사 생성자
		Matrix4(const Matrix4& other);
		~Matrix4() = default;

		// 전치 함수 - 행과 열을 바꾸는 동작 처리
		static Matrix4 Transpose(const Matrix4& matrix);

		// 회전 행렬의 역행렬 함수 - 전치 동작을 역행렬 동작으로 구현
		// 단, 기저 축이 서로 직교(90도)일 때만 처리
		// 이유1: 역행렬 구하기 복잡
		// 이유2: 모든 상황에서 해가 있는 것이 아님
		static Matrix4 InverseRotation(const Matrix4& matrix);

		// [이동 변환 행렬]
		// 계산 결과가 (x + a, y + b, z + c) 형식으로 나오게
		// x 계수가 1이면서 y, z계수는 0이 나오도록
		// -> 4행의 x,y,z, 1에 각 항목의 변화량만큼 곱해준다
		static Matrix4 Translation(float x, float y, float z);
		static Matrix4 Translation(const Vector3& translation);


		// [크기 변환 행렬]
		// 계산 결과가 a * x, b * y, c * z 형식으로 나오게
		// 단위행렬 처럼 x자리엔 x계수만 남기고 나머지 0, 
		static Matrix4 Scale(float x, float y, float z);
		static Matrix4 Scale(const Vector3& scale);
		static Matrix4 Scale(float scale);

		// [회전 변환 행렬] -> 점을 이동시킨다기 보단, 공간을 회전시키는 것
		// 회전 변환은 벡터(위치)의 크기를 유지시킨다. 크기 = 원점에서 해당 위치까지의 거리
		// 편의상 단위 원 안에서 유도. 단위 원 = 반지름이 1인 원
		//
		static Matrix4 RotationX(float angle);
		static Matrix4 RotationY(float angle);
		static Matrix4 RotationZ(float angle);
		static Matrix4 Rotation(float x, float y, float z);
		static Matrix4 Rotation(const Vector3& rotation);

		// TO. Codex
		// 언리얼의 회전 계산이 회전 행렬의 x축을 떼서 쓴다는 얘기를 들었는데, 이걸 설명해줘

		// 내부에서 관리하는 배열의 원시 포인터 반환 함수
		const float* Data() const { return elements; }

		// 행렬 곱 연산자 오버로딩
		Matrix4& operator= (const Matrix4 other);
		Matrix4 operator* (const Matrix4 other) const;
		Matrix4& operator*= (const Matrix4 other);

		// 백터와 행렬 곱
		// 행벡터로 사용해서 처리 -> 벡터가 좌측에 있어야 가능 // 언리얼이 이래서 (OpenGL이나 다른 교재에서는 열벡터로 처리)
		// 행렬 곱 계산 시 행 x 열 로 처리하기 때문
		friend Vector3 operator* (const Vector3& vector, const Matrix4& matrix);

	public:
		// 단위 행렬 상수
		// -> 대각 성분이 1이고, 나머지 성분이 0인 행렬, 행렬 곱을 했을 때 원래 행렬을 반환
		// = 행렬 곱의 항등원
		static const Matrix4 Identity;

	private:
		// 4x4 행렬 변수 선언
		union
		{
			struct
			{
				float m00, m01, m02, m03;		// 1행
				float m10, m11, m12, m13;		// 2행
				float m20, m21, m22, m23;		// 3행
				float m30, m31, m32, m33;		// 3행
			};

			// 배열을 한 번에 선언
			float elements[4 * 4];
		};
	};
}

