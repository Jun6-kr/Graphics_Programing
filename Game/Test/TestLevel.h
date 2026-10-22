#pragma once
#include <GameFramework/Level.h>

class TestLevel : public Craft::Level
{
protected:
	// 초기화 시 사용하는 함수 - 액터 생성 등 처리
	virtual void Initialized() override;
	//virtual void Draw() override;
};
