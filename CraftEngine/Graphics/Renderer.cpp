#include "Renderer.h"
#include <Core/Win32Window.h>
#include <cstdint>
#include <d3dcompiler.h>

namespace Craft
{
	Renderer::Renderer(const Win32Window& window)
	{
		// Device/Context 생성
		CreateDevices();

		// 스왑체인 생성
		CreateSwapChain(window);

		// 렌더 타겟 뷰 생성
		CreateRenderTargetView();

		// 데모 버퍼 생성
		CreateDemoBuffers();

		// 셰이더 컴파일 및 셰이더 객체 생성
		CreateDefaultShaders();

		// 부표트 생성 및 바인딩
		CreateViewport(window.GetWidth(), window.GetHeight());
	}
	Renderer::~Renderer()
	{
		// 실제 리소스
		SafeRelease(vertexBuffer);
		SafeRelease(indexBuffer);
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(inputLayout);

		// 스왑체인이 관리하는 장치 관련 변수
		SafeRelease(renderTargetView);
		SafeRelease(swapChain);
		SafeRelease(context);
		SafeRelease(device);
	}

	void Renderer::Draw(float red, float green, float blue, uint32_t vsync)
	{
		BeginScene(red, green, blue);

		DrawScene();

		EndScene(vsync);
	}

	void Renderer::OnResize(uint32_t width, uint32_t height)
	{
		// 원래 크기 확인
		DXGI_SWAP_CHAIN_DESC desc = {};
		swapChain->GetDesc(&desc);

		// RenderTargetView 해제 - buffer와 RTV가 연결되어 있는 상태면 크기 변경이 안 된다
		SafeRelease(renderTargetView);

		// 백버퍼 크기 변경
		ThrowIfFailed(swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_UNKNOWN, 0), L"Failed to resize back buffer");

		swapChain->GetDesc(&desc);

		// RTV 재생성
		CreateRenderTargetView();

		// 뷰포트 크기 재설정
		CreateViewport(width, height);
	}

	void Renderer::BeginScene(float red, float green, float blue)
	{
		// 그리기 준비
		// 배경 지우기 및 그리기 대상 설정
		context->OMSetRenderTargets(1, &renderTargetView, nullptr);
		float backgroundColor[4] = { red, green, blue, 1.0f };
		context->ClearRenderTargetView(renderTargetView, backgroundColor);
	}

	void Renderer::DrawScene()
	{
		// 입력 설정 -  리소스 바인딩
		uint32_t stride = sizeof(float) * 3;
		uint32_t offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetInputLayout(inputLayout);
		// 토폴로지 -> 정점을 어떻게 이엉서 도형을 만들지 결정하는 방식
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 셰이더 설정
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);

		// 드로우 콜
		context->DrawIndexed(3, 0, 0);
	}

	void Renderer::EndScene(uint32_t vsync)
	{
		// 프론트 - 백 버퍼 교환
		swapChain->Present(vsync, 0);
	}

	void Renderer::CreateDevices()
	{
		uint32_t flag = 0;

#if _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featrureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL selectedFeatureLevel = {};

		// 장치 생성 함수 호출
		/*
		    IDXGIAdapter* pAdapter,											모니터 여러개 쓸 때 몇 번째 모니터를 실행시킬 것인지 정할 때 사용하는 어뎁터
			D3D_DRIVER_TYPE DriverType,			
			HMODULE Software,												그래픽카드 안 사용하고 소프트웨어로 
			UINT Flags,
			( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,		그래픽스 API(Dx11, Dx12 등의 세부 버전) 버전 설정
			UINT FeatureLevels,												
			UINT SDKVersion,												SDK 버전
			ID3D11Device** ppDevice,
			D3D_FEATURE_LEVEL* pFeatureLevel,								선택된 그래픽스 API 반환
			ID3D11DeviceContext** ppImmediateContext
		*/
		ThrowIfFailed(D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			flag,
			featrureLevels,
			_countof(featrureLevels),
			D3D11_SDK_VERSION,
			&device,
			nullptr/*&selectedFeatureLevel*/,
			&context
		), L"Failed to create device");
	}

	void Renderer::CreateSwapChain(const Win32Window& window)
	{
		// 스왑체인 생성을 위한 객체 생성
		IDXGIFactory* factory = nullptr;

		//auto result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
		auto result = CreateDXGIFactory(IID_PPV_ARGS(&factory));

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		/*
		 DXGI_MODE_DESC BufferDesc;		// factory가 관리할 버퍼의 설정?
		 DXGI_SAMPLE_DESC SampleDesc;
		 DXGI_USAGE BufferUsage;
		 UINT BufferCount;
		 HWND OutputWindow;
		 BOOL Windowed;
		 DXGI_SWAP_EFFECT SwapEffect;
		 UINT Flags;
		*/
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

		swapChainDesc.BufferDesc.Width = window.GetWidth();
		swapChainDesc.BufferDesc.Height = window.GetHeight();
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = window.GetHandle();
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		result = factory->CreateSwapChain(device, &swapChainDesc, &swapChain);

		if (FAILED(result))
		{
			__debugbreak();
			return;
		}

		// 해제.
		if (factory)
		{
			factory->Release();
			factory = nullptr;
		}
	}

	void Renderer::CreateRenderTargetView()
	{
		// 백버퍼 (2차원 배열-텍스처) 정보 가져오기
		ID3D11Texture2D* backBuffer = nullptr;
		ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)), L"Failed to get back buffer from swap chain");

		ThrowIfFailed(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView), L"Failed to create RTV");

		// 사용한 후 해제
		SafeRelease(backBuffer);
	}

	void Renderer::CreateDemoBuffers()
	{
		// Temp 구조체 선언
		struct Vector3
		{
			float x, y, z = 0.0f;
		};

		// 삼각형을 이루는 정점 데이터 (배열)
		/*
		*	NDC 좌표 - 중심이 원점
		*   -> 우측/상단 + , 좌측/하단 -, 여기에 z축 까지 - 화면쪽으로 +, 화면 바깥쪽으로 -
		*/

		Vector3 vertices[] =
		{
			Vector3 { 0.0f, 0.5f, 0.5f },
			Vector3 { 0.5f, -0.5f, 0.5f },
			Vector3 { -0.5f, -0.5f, 0.5f },
		};

		// 원시 데이터를 포장해서 그래픽카드에 전달(복사)
		// 전달 매개체가 버퍼

		// 버퍼 구성 정보
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = sizeof(Vector3) * 3;			// 버퍼 크기
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 목적
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA vertexBufferData = {};
		vertexBufferData.pSysMem = vertices;

		ThrowIfFailed(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer), L"Failed to create vertex buffer");

		// 인덱스 원시 데이터 배열
		// 정점의 순서 - 삼각형을 구성할 인덱스 순서
		uint32_t indices[] = { 0, 1, 2 };

		// 버퍼 구성 정보
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * 3;			// 버퍼 크기
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 목적
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA indexBufferData = {};
		indexBufferData.pSysMem = indices;


		ThrowIfFailed(device->CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer), L"Failed to create index buffer");
	}

	void Renderer::CreateDefaultShaders()
	{
		// 셰이더 컴파일 결과 저장용 객체
		ID3DBlob* vertexShaderObject = nullptr;

		// Shader 컴파일
		ThrowIfFailed(D3DCompileFromFile(
			L"HLSLShaders/DefaultVS.hlsl",			// 파일명
			nullptr,								// 
			nullptr,
			"main",									// 엔트리 포인트 - shader 파일의 시작점 함수 이름?
			"vs_5_0",								// 셰이더 모델 버전
			0,
			0,
			&vertexShaderObject,
			nullptr
		), L"Failed to compile vertex shader");

		// 정점 셰이더 객체 생성
		ThrowIfFailed(
			device->CreateVertexShader(
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
		UINT AlignedByteOffset;						// 
		D3D11_INPUT_CLASSIFICATION InputSlotClass;
		UINT InstanceDataStepRate;
		*/
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 정점 셰이더 입력 관련 정보 객체 생성
		ThrowIfFailed(device->CreateInputLayout(
			inputLayoutDesc
			, _countof(inputLayoutDesc)
			, vertexShaderObject->GetBufferPointer()
			, vertexShaderObject->GetBufferSize(),
			&inputLayout), L"Failed to create input layout");


		ID3DBlob* pixelShaderObject = nullptr;

		// Shader 컴파일
		ThrowIfFailed(D3DCompileFromFile(
			L"HLSLShaders/DefaultPS.hlsl",			// 파일명
			nullptr,								// 
			nullptr,
			"main",									// 엔트리 포인트 - shader 파일의 시작점 함수 이름?
			"ps_5_0",								// 셰이더 모델 버전
			0,
			0,
			&pixelShaderObject,
			nullptr
		), L"Failed to compile pixel shader");

		// 픽셀 셰이더 객체 생성
		ThrowIfFailed(
			device->CreatePixelShader(
				pixelShaderObject->GetBufferPointer(),
				pixelShaderObject->GetBufferSize(),
				nullptr,
				&pixelShader)
			, L"Failed to create pixel shader");

		SafeRelease(vertexShaderObject);
		SafeRelease(pixelShaderObject);
	}

	void Renderer::CreateViewport(uint32_t width, uint32_t height)
	{
		// 뷰포트 설정
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 0.0f;

		// 바인딩
		context->RSSetViewports(1, &viewport);
	}
}