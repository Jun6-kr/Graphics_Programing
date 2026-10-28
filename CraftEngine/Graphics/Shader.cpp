#include "Shader.h"
#include "Renderer.h"
#include <Core/Core.h>
#include <d3dcompiler.h>

namespace Craft
{
	Shader::Shader(const std::wstring& shaderName)
	{
		// 빌드 전 파일 (hlsl) -> 빌드 된 파일(cso)로 변경
		std::wstring shaderFolder = L"HLSLShaders/";
		std::wstring vertexPath = shaderFolder + shaderName + std::wstring(L"VS.cso");
		std::wstring pixelPath = shaderFolder + shaderName + std::wstring(L"PS.cso");

		auto& device = Renderer::Get().GetDevice();

		// 셰이더 컴파일 결과 저장용 객체
		ID3DBlob* vertexShaderObject = nullptr;

		/* Shader 컴파일 */
		//ThrowIfFailed(D3DCompileFromFile(
		//	vertexPath.c_str(),			// 파일명
		//	nullptr,
		//	nullptr,
		//	"main",									// 엔트리 포인트 - shader 파일의 시작점 함수 이름?
		//											// [CODEX 답변] 맞다. Compiler가 이 이름의 HLSL 함수부터 해당 Shader Stage를 Compile한다.
		//											// 이름은 main일 필요가 없으며, 파일 안에 존재하는 함수 이름과 일치하면 된다.
		//
		//	"vs_5_0",								// 셰이더 모델 버전
		//	0,
		//	0,
		//	&vertexShaderObject,
		//	nullptr
		//), L"Failed to compile vertex shader");

		// CSO 읽어오기
		ThrowIfFailed(D3DReadFileToBlob(vertexPath.c_str(), &vertexShaderObject), L"Failed to read vertex shader object");

		// 정점 셰이더 객체 생성
		ThrowIfFailed(
			device.CreateVertexShader(
				vertexShaderObject->GetBufferPointer(),
				vertexShaderObject->GetBufferSize(),
				nullptr,
				&vertexShader)
			, L"Failed to create vertex shader");


		/*
		LPCSTR SemanticName
		UINT SemanticIndex
		DXGI_FORMAT Format
		UINT InputSlot								// 정점
		UINT AlignedByteOffset;						// 데이터를 읽을 위치 (단위: 바이트)
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			,{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 정점 셰이더 입력 관련 정보 객체 생성
		ThrowIfFailed(device.CreateInputLayout(
			inputLayoutDesc
			, _countof(inputLayoutDesc)
			, vertexShaderObject->GetBufferPointer()
			, vertexShaderObject->GetBufferSize(),
			&inputLayout), L"Failed to create input layout");


		ID3DBlob* pixelShaderObject = nullptr;

		// Shader 컴파일
		//ThrowIfFailed(D3DCompileFromFile(
		//	pixelPath.c_str(),						// 파일명
		//	nullptr,
		//	nullptr,
		//	"main",									// 엔트리 포인트 - shader 파일의 시작점 함수 이름?
		//											// [CODEX 답변] Pixel Shader에서도 Compile을 시작할 HLSL 함수 이름이다.
		//											// 바로 다음 Target인 ps_5_0이 이 함수를 Pixel Shader로 Compile하도록 Stage를 결정한다.
		//
		//	"ps_5_0",								// 셰이더 모델 버전
		//	0,
		//	0,
		//	&pixelShaderObject,
		//	nullptr
		//), L"Failed to compile pixel shader");

		// 픽셀 셰이더 CSO 파일 로드
		ThrowIfFailed(D3DReadFileToBlob(pixelPath.c_str(), &pixelShaderObject), L"Failed to read pixel shader object");


		// 픽셀 셰이더 객체 생성
		ThrowIfFailed(
			device.CreatePixelShader(
				pixelShaderObject->GetBufferPointer(),
				pixelShaderObject->GetBufferSize(),
				nullptr,
				&pixelShader)
			, L"Failed to create pixel shader");

		SafeRelease(vertexShaderObject);
		SafeRelease(pixelShaderObject);
	}

	Shader::~Shader()
	{
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(inputLayout);
	}

	void Shader::Bind()
	{
		auto& context =Renderer::Get().GetContext();

		// 셰이더 설정
		context.IASetInputLayout(inputLayout);
		context.VSSetShader(vertexShader, nullptr, 0);
		context.PSSetShader(pixelShader, nullptr, 0);
	}
	std::shared_ptr<Shader> Shader::CreateDefaultShader()
	{
		return std::make_shared<Shader>(L"Default");
	}
}
