#include "Transform.h"

namespace Craft
{
	Transform::Transform()
	{
		Update();
	}

	void Transform::Update()
	{
		// 월드 행렬 업데이트 -> 로컬 좌표를 공통 월드 좌표로 변환하는 과정
		// 변환 곱셈 순서 중요함 (SRT)
		// [그래픽스 면접 문제에 자주 출제]
		worldMatrix = Matrix4::Scale(scale) * Matrix4::Rotation(rotation) * Matrix4::Translation(position);
	}

	Vector3 Transform::Right() const
	{
		// 월드의 우측을 액터의 우측으로 회전시킨다? - 회전 행렬의 하나의 행을 사용한다?
		return Vector3::Right * Matrix4::Rotation(rotation);
	}

	Vector3 Transform::Up() const
	{
		return Vector3::Up * Matrix4::Rotation(rotation);;
	}

	Vector3 Transform::Forward() const
	{
		return Vector3::Forward * Matrix4::Rotation(rotation);;
	}
}