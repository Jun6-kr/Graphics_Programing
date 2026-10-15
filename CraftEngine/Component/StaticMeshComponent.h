#pragma once

#include <Component/Component.h>
#include <memory>

namespace Craft
{
	class StaticMesh;
	class Shader;

	class StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent() = default;
		~StaticMeshComponent() = default;

		virtual void Draw() override;

		// Setter
		inline void SetStaticMesh(const std::shared_ptr<StaticMesh>& newMesh) { staticMesh = newMesh; }
		inline void SetShader(const std::shared_ptr<Shader>& newShader) { shader = newShader; }

	private:
		// Static Mesh
		std::shared_ptr<StaticMesh> staticMesh;

		// Shader
		std::shared_ptr<Shader> shader;
	};
}
