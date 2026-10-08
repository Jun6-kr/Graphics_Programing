#include "TransformComponent.h"
#include <Graphics/Renderer.h>

namespace Craft
{
	void TransformComponent::Draw()
	{
		Component::Draw();

		// 트랜스폼 업데이트
		transform.Update();

		// Temp: 렌더러에 월드 행렬 제출
		Renderer::Get().Submit(transform.GetWorldMatrix());
	}
}