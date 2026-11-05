#pragma once

#include <Component/Component.h>

class CameraMovementComponent : public Craft::Component
{
public:
	CameraMovementComponent() = default;
	~CameraMovementComponent() = default;

	virtual void Tick(float deltaTime) override;

	inline void SetMoveSpeed(float newMoveSpeed) { moveSpeed = newMoveSpeed; }
	inline void SetMouseSensitivity(float newMouseSensitivity) { mouseSensitivity = newMouseSensitivity; }

private:
	// 이동 속도 (단위: ??? 모름)
	float moveSpeed = 3.0f;

	// 드래그 감도
	float mouseSensitivity = 0.15f;

	// Pitch(x축) 회전의 각도 제한
	float minPitch = -89.0f;
	float maxPitch = 89.0f;
};

