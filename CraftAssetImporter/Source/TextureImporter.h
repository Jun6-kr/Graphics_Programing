#pragma once

#include <cstdint>
#include <filesystem>
#include <CraftAssets/AssetTypes.h>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <ThirdParty/stb_image.h>

namespace fs = std::filesystem;

// CTexture 파일 쓰기 함수
bool WriteTextureAsset(const uint8_t* pixels, uint32_t width, uint32_t height, const fs::path& outputPath);

// 이미지 원본 파일(jpg, png 등)을 CTexture로 변환(임포트) 하는 함수
bool ImportTexture(const fs::path& inputPath, const fs::path& outputPath);