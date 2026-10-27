#include "Texture.h"
#include <Core/Core.h>

// stb_image 사용을 위한 선언
#define STB_IMAGE_IMPLEMENTATION
#include <ThirdParty/stb_image.h>

namespace Craft
{
	Texture::Texture(const std::string& path)
	{
		LoadFromFile(path);
	}

	Texture::~Texture()
	{
		SafeRelease(texture);
	}

	std::shared_ptr<Texture> Texture::Load(const std::string & path)
	{
		return std::make_shared<Texture>(path);
	}

	void Texture::LoadFromFile(const std::string& path)
	{
		// 이미지 데이터 저장용 변수
		int imageWidth = 0;
		int imageHeight = 0;
		int channelCount = 0;

		// 이미지 로드해서 픽셀 데이터(배열) 저장
		stbi_uc* pixels = stbi_load(path.c_str(), &imageWidth, &imageHeight, &channelCount, STBI_rgb_alpha);

		// 로드 실패 시
		if (nullptr == pixels)
		{
			assert(false);
			return;
		}

		// D3D 텍스처 리소스 생성
	}
}