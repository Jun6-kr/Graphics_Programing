#include "Texture.h"
#include "Renderer.h"
#include "AssetManager.h"
#include <Core/Core.h>
#include <stdexcept>

namespace Craft
{
	Texture::Texture(const std::string& path)
	{
		LoadFromFile(path);
	}

	Texture::~Texture()
	{
		SafeRelease(texture);
		SafeRelease(shaderResourceView);
		SafeRelease(samplerState);
	}

	void Texture::Bind(uint32_t slot) const
	{
		auto& context = Renderer::Get().GetContext();

		// 텍스처 (셰이더 리소스) 바인딩
		context.PSSetShaderResources(slot, 1, &shaderResourceView);

		// 샘플러 바인딩
		context.PSSetSamplers(slot, 1, &samplerState);
	}

	std::shared_ptr<Texture> Texture::Load(const std::string & path)
	{
		return std::make_shared<Texture>(path);
	}

	void Texture::LoadFromFile(const std::string& path)
	{
		const auto asset = AssetManager::Get().LoadTexture(path);

		// 로드 실패 처리
		if (nullptr == asset)
		{
			throw std::runtime_error("Failed to load texture asset: " + path);
			return;
		}

		// 무결성 검증 - 채널 수가 4개인지 확인
		if (asset->channels != 4)
		{
			throw std::runtime_error("Invalid ctexture asset");
			return;
		}

		// D3D 리고스 생성
		LoadFromPixels(asset->pixels.data(), asset->width, asset->hegiht);
	}

	void Texture::LoadFromPixels(const uint8_t* pixels, uint32_t textureWidth, uint32_t textureHeight)
	{
		// 너비 / 높이 값 저장
		width = textureWidth;
		height = textureHeight;

		// D3D 텍스처 리소스 생성
		auto& device = Renderer::Get().GetDevice();

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA textureData = {};
		textureData.pSysMem = pixels;
		textureData.SysMemPitch = width * 4;	// 2차원 배열의 가로 너비 -> [codex] * 4를 해주는 이유는 RGBA라서?

		auto result = device.CreateTexture2D(&textureDesc, &textureData, &texture);

		// 성공/실패 여부 확인
		ThrowIfFailed(result, L"Failed to create texture2d");

		// 셰이더 리소스 뷰 생성
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;

		ThrowIfFailed(device.CreateShaderResourceView(texture, &srvDesc, &shaderResourceView), L"Failed to create shader resource view");

		// 샘플러 스테이트 - 텍스처를 어떤 방식으로 읽을지(샘플링 할지)를 설정하는 객체
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;		// 줄일 때, 확장할 때 리니어로 할것이다
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;			// 좌상단 0,0 / 우하단 1,1 / 깊이(w)
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		ThrowIfFailed(device.CreateSamplerState(&samplerDesc, &samplerState), L"Failed to create sampler state");
	}
}