#pragma once

#include <GameFramework/Actor.h>

// 전방선언
namespace Craft
{
	class StaticMesh;
}

class TestActor : public Craft::Actor
{
public:
	virtual void Tick(float deltaTime) override;

public:
	float speed = 0.0f;

private:
	// @Temp : 액터에서 사용할 스태틱 메시
	std::shared_ptr<Craft::StaticMesh> staticMesh;
};
