#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>

void TestActor::Initialized()
{
	Actor::Initialized();

	AddComponent<Craft::StaticMeshComponent>();
}

void TestActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	// 회전
	GetTransform().rotation.z += speed * deltaTime;
}
