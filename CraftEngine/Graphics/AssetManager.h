#pragma once

#include <Core/Core.h>
#include <CraftAssets/AssetTypes.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Craft
{
	class AssetManager : public TGlobalAccessible<AssetManager>
	{
	public:
		AssetManager(std::filesystem::path root = "Assets/Imported");
		~AssetManager();

		// 메시 로드 함수
		std::shared_ptr<MeshAsset> LoadMesh(const std::string& assetName);

		// 텍스처 로드 함수
		std::shared_ptr<TextureAsset> LoadTexture(const std::string& assetName);

	protected:
		std::filesystem::path Resolve(const std::string& assetName) const;

	private:
		// 애셋 관리자가 사용하는 기본 루트 경로
		std::filesystem::path root;

		std::unordered_map<std::string, std::shared_ptr<MeshAsset>>  meshes;

		std::unordered_map<std::string, std::shared_ptr<TextureAsset>> textures;
	};
}

