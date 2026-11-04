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

	if (input.GetMouseButtonDown(0))
	{
		std::cout << "Left Mouse Down\n";
	}

	if (input.GetMouseButtonUp(0))
	{
		std::cout << "Left Mouse Up\n";
	}

	if (input.GetMouseButton(0))
	{
		std::cout << "Mouse Delta: "
			<< input.GetMouseDeltaX()
			<< ", "
			<< input.GetMouseDeltaY()
			<< "\n";
	}

	if (input.GetMouseButtonDown(1))
	{
		std::cout << "Right Mouse Down\n";
	}

	if (input.GetMouseButtonDown(1))
	{
		std::cout << "Right Mouse Down\n";
	}
}
