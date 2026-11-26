#include "MeshImporter.h"
#include "TextureImporter.h"
#include "Utility.h"

#include <fstream>
#include <iostream>

bool WriteMeshAsset(const Craft::MeshAsset& mesh, const fs::path& outputPath)
{
    // 상위 폴더 생성
    fs::create_directories(outputPath.parent_path());

    // 출력용도로 파일 스트림 생성
    std::ofstream output(outputPath, std::ios::binary);

    if (!output.is_open())
    {
        std::cerr << "Failed to create mesh asset: " << outputPath << "\n";
        return false;
    }

    // 시그니처 기록
    output.write(Craft::cMeshMagic, sizeof(Craft::cMeshMagic));

    // 애셋 버전 기록
    WritePod(output, Craft::assetVersion);

    // 메시 애셋 데이터 기록
    const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
    const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());

    WritePod(output, vertexCount);
    WritePod(output, indexCount);

    // 정점 배열 데이터 기록
    output.write(reinterpret_cast<const char*>(mesh.vertices.data()), sizeof(Craft::AssetVertex) * vertexCount);

    // 인덱스 배열 데이터 기록
    output.write(reinterpret_cast<const char*>(mesh.indices.data()), sizeof(uint32_t) * indexCount);
    
    // 서브 메시 데이터 기록
    const uint32_t subMeshCount = static_cast<uint32_t>(mesh.subMeshes.size());
    WritePod(output, subMeshCount);

    // 서브 메시 데이터 기록
    for (const Craft::AssetSubMesh& subMesh : mesh.subMeshes)
    {
        WritePod(output, subMesh.indexOffset);
        WritePod(output, subMesh.indexCount);
        WritePod(output, subMesh.textureAsset);
    }

    return true;
}

bool ImportMesh(const fs::path& inputPath, const fs::path& outputPath, const fs::path& outputRoot)
{
	// FBX는 씬 단위로 저장됨.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(
		inputPath.string(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ImproveCacheLocality |
		aiProcess_ConvertToLeftHanded
	);

	// 메시 정보 가져오기.
	if (!scene || !scene->HasMeshes())
	{
		std::cerr
			<< "Failed to import mesh: "
			<< inputPath << "\n"
			<< importer.GetErrorString() << "\n";

		return false;
	}

	// 우리가 정의한 포맷에 저장.
	Craft::MeshAsset meshAsset;

	// 씬에 저장된 메시를 순회하면서 처리.
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		// 현재 순번의 메시 로드.
		const aiMesh* sourceMesh = scene->mMeshes[meshIndex];

		// 서브 메시 데이터 저장.
		Craft::AssetSubMesh subMesh;

		// 인덱스 오프셋 기록.
		subMesh.indexOffset
			= static_cast<uint32_t>(meshAsset.indices.size());

		// 이번 서브 메시가 읽기 시작해야하는 정점 순번.
		const uint32_t vertexBase
			= static_cast<uint32_t>(meshAsset.vertices.size());

		// 메시에 저장된 정점 순회하면서 정보 추출 및 저장.
		for (uint32_t vertexIndex = 0;
			vertexIndex < sourceMesh->mNumVertices;
			++vertexIndex)
		{
			// 정점.
			Craft::AssetVertex vertex;

			const aiVector3D& position = sourceMesh->mVertices[vertexIndex];

			// 정점 위치 저장.
			vertex.position = { position.x, position.y, position.z };

			// UV 저장.
			if (sourceMesh->HasTextureCoords(0))
			{
				const aiVector3D& uv
					= sourceMesh->mTextureCoords[0][vertexIndex];

				vertex.uv = { uv.x, uv.y };
			}

			// 노멀.
			if (sourceMesh->HasNormals())
			{
				const aiVector3D& normal
					= sourceMesh->mNormals[vertexIndex];

				vertex.normal = { normal.x, normal.y, normal.z };
			}

			// 탄젠트 공간 데이터.
			if (sourceMesh->HasTangentsAndBitangents())
			{
				const aiVector3D& tangent
					= sourceMesh->mTangents[vertexIndex];

				const aiVector3D& bitangent
					= sourceMesh->mBitangents[vertexIndex];

				vertex.tangent = { tangent.x, tangent.y, tangent.z };
				vertex.bitangent = { bitangent.x, bitangent.y, bitangent.z };
			}

			// 정점 배열에 추가.
			meshAsset.vertices.emplace_back(vertex);
		}

		// 인덱스 정보 추가.
		// 면 정보를 순회하면서 인덱스 추가.
		for (uint32_t faceIndex = 0; faceIndex < sourceMesh->mNumFaces; ++faceIndex)
		{
			// 면(Face) 가져오기.
			const aiFace& face = sourceMesh->mFaces[faceIndex];

			// 면을 이루는 점의 수 확인.
			if (face.mNumIndices != 3)
			{
				continue;
			}

			// 인덱스 추가.
			meshAsset.indices.emplace_back(vertexBase + face.mIndices[0]);
			meshAsset.indices.emplace_back(vertexBase + face.mIndices[1]);
			meshAsset.indices.emplace_back(vertexBase + face.mIndices[2]);
		}

		// 현재 서브 메시의 인덱스 개수 구하기.
		subMesh.indexCount
			= static_cast<uint32_t>(meshAsset.indices.size()) - subMesh.indexOffset;

		// 서브 메시에서 사용하는 디퓨즈(기본/베이스 컬러) 텍스처 애셋 경로 저장.
		subMesh.textureAsset = ImportDiffuseTexture(
			scene,
			sourceMesh,
			inputPath,
			outputRoot,
			outputPath
		);

		// 서브 메시 추가.
		meshAsset.subMeshes.emplace_back(subMesh);
	}

	// FBX에서 로드한 정보를 커스텀 애셋으로 저장.
	if (!WriteMeshAsset(meshAsset, outputPath))
	{
		return false;
	}

	std::cout << "Import mesh: " << inputPath << " -> " << outputPath << "\n";

	return true;
}

std::string ImportDiffuseTexture(const aiScene* scene, const aiMesh* sourceMesh, const fs::path& meshSourcePath, const fs::path& outputRoot, const fs::path& meshOutputPath)
{
	// FBX가 텍스처 정보를 가지려면 머티리얼을 사용한다는 얘기
	if (!scene->HasMaterials() || sourceMesh->mMaterialIndex >= scene->mNumMaterials)
	{
	    return std::string();
	}

	// 머티리얼 가져오기
	const aiMaterial* material = scene->mMaterials[sourceMesh->mMaterialIndex];
	if (material->GetTextureCount(aiTextureType_DIFFUSE) == 0)
	{
		return {};
	}

	// 텍스처 이름 가져오기
	aiString textureName;
	if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) != aiReturn_SUCCESS)
	{
		return {};
	}

	// 원본 텍스처 경로
	fs::path sourceTexture = meshSourcePath.parent_path() / fs::path(textureName.C_Str());

	// 경로 확인
	if (!fs::exists(sourceTexture))
	{
		return {};
	}

	// 임포트 처리 후 저장할 경로
	fs::path outputTexture = meshOutputPath.parent_path() / fs::path(sourceTexture.stem());

	//확장자를 커스텀 텍스처로 변경
	outputTexture.replace_extension(".ctexture");

	if (!ImportTexture(sourceTexture, outputTexture))
	{
		return {};
	}

	// generic string 함수는 경로 구분자를 /로 반환
	// -> Windows의 경우 경로 구분자가\인데, 이걸 변환
	return fs::relative(outputTexture, outputRoot).generic_string();
}
