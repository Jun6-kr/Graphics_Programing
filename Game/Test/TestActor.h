#pragma once
#include <GameFramework/Actor.h>

class TestActor : public Craft::Actor
{
public:
	virtual void Tick(float deltaTime) override;

public:
	float speed = 0.0f;

};
