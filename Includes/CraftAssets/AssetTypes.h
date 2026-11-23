#pragma once

#include <cstdint>
#include <vector>

// 임포트 및 엔진에서 사용할 커스텀 공용 에셋 타입
namespace Craft
{
	// 시그니처(텍츠서/메시 구분 용도)
	inline constexpr char cMeshMagic[4] = { 'M', 'E', 'S', 'H' };
	inline constexpr char cTextureMagic[4] = { 'C', 'T', 'E', 'X' };

	inline constexpr uint32_t  assetVersion = 1;

	// 텍스처 에셋 정의
	struct TextureAsset
	{
		uint32_t width = 0;
		uint32_t hegiht = 0;
		uint32_t channels = 4;
		std::vector<uint8_t> pixels;		// 알아서 채널 단위로 잘라서 써야 함
	};
}