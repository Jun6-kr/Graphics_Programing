#include "TestLevel.h"
#include "TestActor.h"
#include <Component/TransformComponent.h>

using namespace Craft;

void TestLevel::Initialized()
{
	Level::Initialized();

	// 액터 생성
	auto left = SpawnActor<TestActor>();
	left->speed = 60.f;
	left->GetTransform().scale = Vector3::One * 0.35f;
	left->GetTransform().position.x = -0.5f;

	auto right = SpawnActor<TestActor>();
	right->speed = 600.f;
	right->GetTransform().scale = Vector3::One * 0.6f;
	right->GetTransform().position.x = 0.5f;
}
