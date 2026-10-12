#include "TestActor.h"
#include <Component/TransformComponent.h>

void TestActor::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	// 회전
	GetTransform().rotation.z += speed * deltaTime;
}
