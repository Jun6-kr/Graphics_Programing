#pragma once

#include <Core/Core.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace Craft
{
	class Texture;

	class TextureManager : public TGlobalAccessible<TextureManager>
	{
	public:
		TextureManager() = default;
		~TextureManager() = default;

		// 텍스처 로드 함수
		std::shared_ptr<Texture> LoadTexture(const std::string& texturePath);

	private:
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
	};
}

