#pragma once

#include "Matrix4.h"
#include <Core/Core.h>

namespace Craft
{
	// 물체의 변환 정보를 관리하는 클래스
	class Transform
	{
	public:
		Transform();
		~Transform() = default;

		// 월드 변환 행렬 업데이트 함수
		void Update();

		// 물체 기준 방향 반환 함수
		Vector3 Right() const;
		Vector3 Up() const;
		Vector3 Forward() const;

		// 월드 변환 행렬 반환 Getter
		inline const Matrix4& GetWorldMatrix() const { return worldMatrix; }

	public:
		Vector3 position = Vector3::Zero;
		Vector3 rotation = Vector3::Zero;
		Vector3 scale = Vector3::One;

	private:
		// 월드 변환 행렬 (위치/회전/크기)
		Matrix4 worldMatrix;
	};
}