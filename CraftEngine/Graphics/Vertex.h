#pragma once

#include <Math/Vector3.h>
#include <Math/Vector2.h>

namespace Craft
{
	// Mesh의 기본 정보인 정점 구조체
	struct Vertex
	{
		// 위치
		Vector3 position;

		// 텍스처 좌표(uv)
		Vector2 uv;
	};
}