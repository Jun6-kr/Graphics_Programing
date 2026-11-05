#include "CameraMovementComponent.h"
#include <Core/Input.h>
#include <Gameframework/Actor.h>
#include <Math/Transform.h>
#include <Math/MathDefines.h>
#include <algorithm>

using namespace Craft;

void CameraMovementComponent::Tick(float deltaTime)
{
	Component::Tick(deltaTime);

	// 액터에 접근
	auto actor = GetOwner();
	if (nullptr == actor)
	{
		return;
	}

	// 트랜스폼 얻기
	Transform& transform = actor->GetTransform();

	// Input 객체 참조
	Input& input = Input::Get();

	// 이동 방향 변수
	Vector3 moveDirection = Vector3::Zero;

	// [이동 입력 처리]
	// WASD - 로컬 기준 방향 / QE - 월드 기준 Up 방향
	if (input.GetKey('W'))
	{
		moveDirection += transform.Forward();
	}
	if (input.GetKey('S'))
	{
		moveDirection -= transform.Forward();
	}
	if (input.GetKey('D'))
	{
		moveDirection += transform.Right();
	}
	if (input.GetKey('A'))
	{
		moveDirection -= transform.Right();
	}

	if (input.GetKey('E'))
	{
		moveDirection += Vector3::Up;
	}
	if (input.GetKey('Q'))
	{
		moveDirection -= Vector3::Up;
	}

	// 입력에 따라 이동 처리
	if (moveDirection.LengthSquared() > KindaSmallNumber)
	{
		// 이동 처리 = 위치 + 벡터 (방향과 크기)
		transform.position += moveDirection.Normalized() * moveSpeed * deltaTime;
	}

	// 회전 처리 (마우스 드래그 + 왼쪽 버튼 눌렀을 때만)
	if (input.GetMouseButton(0))
	{
		// Yaw 회전 적용 (회전각도 제한 없음)
		transform.rotation.y += input.GetMouseDeltaX() * mouseSensitivity;

		// Pitch 회전 적용 (회전 각도 제한 있음)
		float pitch = transform.rotation.x + input.GetMouseDeltaY() * mouseSensitivity;

		// 제한 처리한 뒤 설정
		transform.rotation.x = std::clamp(pitch, minPitch, maxPitch);
	}
}
