#include "StaticMeshComponent.h"
#include <Graphics/Renderer.h>
#include <GameFramework/Actor.h>
#include <Math/Transform.h>

namespace Craft
{
	void StaticMeshComponent::Draw()
	{
		Component::Draw();

		// 소유자 액터 가져오기
		auto actor = GetOwner();

		if (nullptr == actor || nullptr == shader || nullptr == staticMesh)
		{
			return;
		}

		const Transform& transform = actor->GetTransform();

		// 렌더러에 제출
		Renderer::Get().Submit(staticMesh, shader, texture, transform.GetWorldMatrix());
	}
}
