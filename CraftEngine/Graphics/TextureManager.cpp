#include "TextureManager.h"
#include "Texture.h"

namespace Craft
{
	std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& texturePath)
	{
		// 검색 후 있으면 리소스 반환
		auto result = textures.find(texturePath);
		if (result != textures.end())
		{
			return result->second;
		}

		auto newTexture = Texture::Load(texturePath);
		textures.emplace(texturePath, newTexture);
		return newTexture;
	}
}