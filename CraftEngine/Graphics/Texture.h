#pragma once

namespace Craft
{
	// 이미지 파일을 로드하고, D3D 텍스처를 관리하는 클래스
	class Texture
	{
	public:
		Texture(const std::string& path);
		~Texture();

		// 셰이더에 바인딩(연결)하는 함수
		void Bind(uint32_t slot = 0) const;

		static std::shared_ptr<Texture> Load(const std::string& path);

	private:
		void LoadFromFile(const std::string& path);
		// 전달된 이미지 픽셀 데이터(.ctexture)를 기반으로 GPU 리소스 생성하는 함수
		void LoadFromPixels(const uint8_t* pixels, uint32_t textureWidth, uint32_t textureHeight);

	private:
		// 이미지 크기 (너비/높이)
		uint32_t width = 0;
		uint32_t height = 0;

		// D3D texture 리소스
		ID3D11Texture2D* texture = nullptr;

		// 텍스처 바인딩을 위한 뷰
		ID3D11ShaderResourceView* shaderResourceView = nullptr;

		// 샘플러 스테이트
		ID3D11SamplerState* samplerState = nullptr;
	};
}

