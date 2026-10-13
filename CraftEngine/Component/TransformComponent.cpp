#include "TransformComponent.h"

namespace Craft
{
	void TransformComponent::Draw()
	{
		Component::Draw();

		// 트랜스폼 업데이트
		transform.Update();
	}
}