#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Graphics/Renderer.h>

void TestActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	// 회전
	GetTransform().rotation.z += speed * deltaTime;
}

void TestActor::Draw()
{
	Actor::Draw();

	// 월드 행렬을 얻어오기 위한 트랜스폼
	const Craft::Transform& transform = GetTransform();

	// 렌더러에 그리는데 필요한 데이터 제출
	Craft::Renderer::Get().Submit(staticMesh, shader, transform.GetWorldMatrix());
}
