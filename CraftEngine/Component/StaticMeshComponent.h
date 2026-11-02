#pragma once

#include <Component/Component.h>
#include <memory>

namespace Craft
{
	class StaticMesh;
	class Material;

	class StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent() = default;
		~StaticMeshComponent() = default;

		virtual void Draw() override;

		// Setter
		inline void SetStaticMesh(const std::shared_ptr<StaticMesh>& newMesh) { staticMesh = newMesh; }
		inline void SetMaterial(const std::shared_ptr<Material>& newMaterial) { material = newMaterial; }

	private:
		// Static Mesh
		std::shared_ptr<StaticMesh> staticMesh;

		// Material (Shader + Texture)
		std::shared_ptr<Material> material;
	};
}
