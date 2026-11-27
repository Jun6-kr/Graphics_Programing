#include "AssetManager.h"

#include <fstream>
#include <stdexcept>
#include <cassert>

namespace Craft
{
	namespace
	{
		// 파일 읽기 함수
		template<typename T>
		void ReadPod(std::ifstream& input, T& value)
		{
			input.read(reinterpret_cast<char*>(&value), sizeof(T));
		}

		// 문자열 읽는 함수
		std::string ReadString(std::ifstream& input)
		{
			// 쓸 때와 같은 순서로 읽어야 한다
			// 크기 -> 데이터 순서

			// 크기 읽기
			uint32_t size = 0;
			ReadPod(input, size);

			// 문자 배열 읽어서 채우기
			std::string value(size, 0);
			input.read(value.data(), size);

			return value;
		}
	}

	AssetManager::AssetManager(std::filesystem::path root)
		: root(root)
	{
	}

	AssetManager::~AssetManager()
	{
	}

	std::shared_ptr<MeshAsset> AssetManager::LoadMesh(const std::string & assetName)
	{
		auto result = meshes.find(assetName);
		if (result != meshes.end())
		{
			return result->second;
		}

		// 없으면 파일에서 로드
		std::ifstream input(Resolve(assetName), std::ios::binary);

		// 파일 읽기 확인
		if (!input.is_open())
		{
			throw std::runtime_error("Failed to open mesh asset: " + assetName);
			return {};
		}

		// 메시 애셋 저장한 순서대로 데이터 읽어오기
		auto mesh = std::make_shared<MeshAsset>();
		char magic[4] = {};
		input.read(magic, sizeof(magic));

		// 2. 버전
		uint32_t version = 0;
		ReadPod(input, version);

		// 읽은 데이터 확인(무결성 검증) - 동일할 시 0 반환
		if (std::memcmp(magic, cMeshMagic, sizeof(cMeshMagic)) != 0
			|| version != assetVersion)
		{
			throw std::runtime_error("Failed to open texture asset: " + assetName);
			return {};
		}

		// 3. 정점 & 4. 인덱스
		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;
		ReadPod(input, vertexCount);
		ReadPod(input, indexCount);

		mesh->vertices.resize(vertexCount);
		input.read(reinterpret_cast<char*>(mesh->vertices.data()), sizeof(AssetVertex) * vertexCount);
		mesh->indices.resize(indexCount);
		input.read(reinterpret_cast<char*>(mesh->indices.data()), sizeof(uint32_t) * indexCount);

		// 5. 서브 메시
		uint32_t subMeshCount = 0;
		ReadPod(input, subMeshCount);

		mesh->subMeshes.resize(subMeshCount);

		for (AssetSubMesh& subMesh : mesh->subMeshes)
		{
			ReadPod(input, subMesh.indexOffset);
			ReadPod(input, subMesh.indexCount);
			subMesh.textureAsset = ReadString(input);
		}

		// 테이블에 추가
		meshes.emplace(assetName, mesh);

		return mesh;
	}
	
	std::shared_ptr<TextureAsset> AssetManager::LoadTexture(const std::string& assetName)
	{
		// 이미 가진 애셋인지 확인 후 반환
		auto result = textures.find(assetName);
		if (result != textures.end())
		{
			return result->second;
		}

		// 새로운 애셋이라면 로드 후 테이블에 추가하고 반환
		// 파일 열기
		std::ifstream input(Resolve(assetName), std::ios::binary);

		// 파일 읽기 확인
		if (!input.is_open())
		{
			throw std::runtime_error("Failed to open texture asset: " + assetName);
			return {};
		}

		// 텍스처 애셋 저장한 순서대로 데이터 읽어오기
		auto texture = std::make_shared<TextureAsset>();
		
		// 1. 매직 문자 - 얘는 문자'열'이 아니다 -> 문자열은 마지막에 널문자 추가되어야 함
		char magic[4] = {};
		input.read(magic, sizeof(magic));

		// 2. 버전
		uint32_t version = 0;
		ReadPod(input, version);

		// 읽은 데이터 확인(무결성 검증) - 동일할 시 0 반환
		if (std::memcmp(magic, cTextureMagic, sizeof(cTextureMagic)) != 0
			|| version != assetVersion)
		{
			throw std::runtime_error("Failed to open texture asset: " + assetName);
			return {};
		}

		// 3. 너비
		ReadPod(input, texture->width);

		// 4. 높이
		ReadPod(input, texture->hegiht);

		// 5. 채널
		ReadPod(input, texture->channels);

		// 6. 픽셀 데이터
		const size_t byteCount = static_cast<size_t>(texture->width) * texture->hegiht * texture->channels;
		texture->pixels.resize(byteCount);
		input.read(reinterpret_cast<char*>(texture->pixels.data()), static_cast<std::streamsize>(byteCount));


		// 로드 완료된 커스텀 텍스처를 테이블에 추가
		textures.emplace(assetName, texture);

		return texture;
	}
	
	std::filesystem::path AssetManager::Resolve(const std::string& assetName) const
	{
		// 전달 받은 값으로 경로 생성
		std::filesystem::path path(assetName);

		// 절대 경로면 그대로 반환 
		if (path.is_absolute())
		{
			return path;
		}

		// 이미 찾을 수 있으면 반환
		if (std::filesystem::exists(path))
		{
			return path;
		}

		// 루트 경로 고려해서 추가 검색
		std::filesystem::path direct = root / path;
		if (std::filesystem::exists(direct))
		{
			return direct;
		}

		return direct;
	}
}
