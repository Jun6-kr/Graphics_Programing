#pragma once
#include <Component/Component.h>
#include <Math/Transform.h>

namespace Craft
{
	// 트랜스폼 정보를 관리하는 객체
	class TransformComponent : public Component
	{
	public:
		TransformComponent() = default;
		~TransformComponent() = default;

		// Draw 함수 오버라이드
		virtual void Draw() override;

		// Transform Getter
		inline Transform& GetTransform() { return transform; }
		inline const Transform& GetTransform() const { return transform; }

	private:
		// 트랜스폼
		Transform transform;
	};
}