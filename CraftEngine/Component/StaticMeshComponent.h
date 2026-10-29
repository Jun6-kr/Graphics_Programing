#pragma once

#include <Component/Component.h>
#include <memory>

namespace Craft
{
	class StaticMesh;
	class Shader;
	class Texture;

	class StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent() = default;
		~StaticMeshComponent() = default;

		virtual void Draw() override;

		// Setter
		inline void SetStaticMesh(const std::shared_ptr<StaticMesh>& newMesh) { staticMesh = newMesh; }
		inline void SetShader(const std::shared_ptr<Shader>& newShader) { shader = newShader; }
		inline void SetTexture(const std::shared_ptr<Texture>& newTexture) { texture = newTexture; }

	private:
		// Static Mesh
		std::shared_ptr<StaticMesh> staticMesh;

		// Shader
		std::shared_ptr<Shader> shader;

		// Texture
		std::shared_ptr<Texture> texture;
	};
}
