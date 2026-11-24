#pragma once

#include <CraftAssets/AssetTypes.h>

#include <filesystem>
#include <string>

// Assimp 라이브러리 헤더
#include <Assimp/Importer.hpp>
#include <Assimp/material.h>
#include <Assimp/postprocess.h>
#include <Assimp/scene.h>

namespace fs = std::filesystem;

// 메시 애셋을 커스텀 확장자 파일로 기록하는 함수
bool WriteMeshAsset(const Craft::MeshAsset& mesh, const fs::path& outputPath);

// 메시 임포트 처리 함수
bool ImportMesh(const fs::path& inputPath, const fs::path& outputPath, const fs::path& outputRoot);

// 기본 색상을 담당하는 텍스처 임포트 함수 (언리얼의 base color)
std::string ImportDiffuseTexture(const aiScene* scene, const aiMesh* sourceMesh, const fs::path& meshSourcePath, const fs::path& outputRoot, const fs::path& meshOutputPath);
