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
		// [CODEX 답변]
		// 단위 Right 벡터를 현재 회전 행렬로 변환하면 Actor의 회전된 Local Right 방향을 얻는다.
		// 현재 행벡터 규약에서는 (1, 0, 0) * Rotation 결과가 회전 행렬의 첫 번째 행과 같으므로
		// 회전 행렬의 X축 기저를 꺼내 쓰는 것과 같은 의미다. 방향만 필요하므로 Translation은 적용하지 않는다.
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
