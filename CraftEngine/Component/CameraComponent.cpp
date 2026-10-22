#include "CameraComponent.h"
#include <Graphics/Renderer.h>
#include <Core/Engine.h>
#include <Math/Transform.h>

namespace Craft
{
	void CameraComponent::Draw()
	{
		// 뷰변환 행렬 생성 및 전달
		auto actor = GetOwner();
		const Transform& transform = actor->GetTransform();

		Matrix4 view = Matrix4::InverseTranslationRotation(transform.position, transform.rotation);

		// 투영 변환 행렬 생성 및 전달
		const float width = static_cast<float>(Engine::Get().GetClientWidth());
		const float height = static_cast<float>(Engine::Get().GetClientHeight());

		if (width == 0.0f || height == 0.0f)
		{
			return;
		}

		Matrix4 projection = Matrix4::Perspective(fieldOfView, width, height, nearDistance, farDistance);

		Renderer::Get().SubmitCamera(view, projection);
	}

	void CameraComponent::SetProjection(float fieldOfView, float nearDistance, float farDistance)
	{
		this->fieldOfView = fieldOfView;
		this->nearDistance = nearDistance;
		this->farDistance = farDistance;
	}
}
