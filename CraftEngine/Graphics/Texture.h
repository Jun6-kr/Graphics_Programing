#pragma once

namespace Craft
{
	// 이미지 파일을 로드하고, D3D 텍스처를 관리하는 클래스
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		static std::shared_ptr<Texture> Load(const std::string& path);

	private:
		void LoadFromFile(const std::string& path);

	private:
		// 이미지 크기 (너비/높이)
		uint32_t width = 0;
		uint32_t height = 0;

		// D3D texture 리소스
		ID3D11Texture2D* texture = nullptr;
	};
}

