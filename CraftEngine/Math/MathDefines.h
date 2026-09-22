#pragma once

// #include <DirectXMath.h>

namespace Craft
{
	// PI
	constexpr float Pi = 3.141592f;

	// 0에 가까운 수 정의
	constexpr float KindaSmallNumber = 1e-4f;


	// 라디안 <-> 각도 변환 값
	// 2Pi = 360도 -> pi 라디안 = 180도
	// 
	constexpr float DegreesToRadians = Pi / 180.0f;
	constexpr float RadiansToDegress = 180.0f / Pi;
}