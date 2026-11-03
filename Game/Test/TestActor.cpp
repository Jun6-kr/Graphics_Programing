#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>
#include <Core/Input.h>
#include <iostream>

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

	// w키 입력 테스트
	Craft::Input& input = Craft::Input::Get();
	if (input.GetKeyDown('W'))
	{
		std::cout << "W Key Donw\n";
	}

	if (input.GetKeyUp('W'))
	{
		std::cout << "W Key Up\n";
	}

	if (input.GetKey('W'))
	{
		std::cout << "W Key Hold\n";
	}
}
