#pragma once

#include <d3d11.h>
#include <string>

namespace Craft
{
	// D3D11 셰이더 객체를 관리하는 클래스
	class Shader
	{
	public:
		Shader(const std::wstring& vertexPath, const std::wstring& pixelPath);
		~Shader();

		// 그래픽스 파이프라인에 셰이더 객체 바인딩
		void Bind();

	private:
		// 정점 셰이더
		ID3D11VertexShader* vertexShader = nullptr;

		// 픽셀 셰이더
		ID3D11PixelShader* pixelShader = nullptr;

		// 입력 레이아웃 (정점 셰이더 입력의 명세서)
		ID3D11InputLayout* inputLayout = nullptr;
	};
}