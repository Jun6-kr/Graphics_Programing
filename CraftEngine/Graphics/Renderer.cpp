#include "Renderer.h"
#include "StaticMesh.h"
#include "Shader.h"
#include <Core/Win32Window.h>
#include <d3dcompiler.h>
#include <cstdint>
#include <cassert>

namespace Craft
{
	Renderer::Renderer(const Win32Window& window)
	{
		assert(!instance);
		instance = this;

		// Device/Context 생성
		CreateDevices();

		// 스왑체인 생성
		CreateSwapChain(window);

		// 렌더 타겟 뷰 생성
		CreateRenderTargetView();

		// 부표트 생성 및 바인딩
		CreateViewport(window.GetWidth(), window.GetHeight());

		// 트랜스폼 버퍼 생성
		CreateTansformBuffer();

		// 카메라 버퍼 생성
		CreateCameraBuffer();
	}
	Renderer::~Renderer()
	{
		instance = nullptr;

		// 사용하던 리소스 먼저 -> 그 다음 장치 해제
		SafeRelease(transformBuffer);
		SafeRelease(cameraBuffer);

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

	void Renderer::Submit(const std::shared_ptr<StaticMesh>& mesh, const std::shared_ptr<Shader>& shader, const Matrix4& worldMatrix)
	{
		// 삽입할 렌더 명령 생성
		RenderCommand command;
		command.worldMatrix = worldMatrix;
		command.mesh = mesh;
		command.shader = shader;

		// 렌더 목록에 추가
		renderCommandList.emplace_back(command);
	}

	void Renderer::SubmitCamera(const Matrix4& viewMatrix)
	{
		cameraData.viewMatrix = viewMatrix;
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

	Renderer& Renderer::Get()
	{
		assert(instance && "instance should not be null here");

		return *instance;
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
		// 정점 세이더에 상수 버퍼(트랜스폼 버퍼) 바인딩
		context->VSSetConstantBuffers(0, 1, &transformBuffer);

		// 카메라 버퍼 업데이트
		UpdateCameraBuffer();

		// 정점 세이더에 상수 버퍼(카메라 버퍼) 바인딩
		context->VSSetConstantBuffers(1, 1, &cameraBuffer);		// 시작 slot(b1), 전달할 버퍼 수, 버퍼

		// 렌더 명령 처리
		for (const RenderCommand& command : renderCommandList)
		{
			DrawCommand(command);
		}

		// 랜더 명령 목록 정리
		renderCommandList.clear();
	}

	void Renderer::DrawCommand(const RenderCommand& command)
	{
		// 메시 / 셰이더 설정이 안됐으면 종료
		if (nullptr == command.mesh || nullptr == command.shader)
		{
			return;
		}

		// 메시 바인딩
		command.mesh->Bind();

		// 셰이더 바인딩
		command.shader->Bind();

		// 트랜스폼 데이터 업데이트
		UpdateTrnasformBuffer(command.worldMatrix);

		// 드로우콜
		context->DrawIndexed(command.mesh->GetIndexCount(), 0, 0);
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
		 // [CODEX 답변]
		 // Factory가 직접 관리하는 설정이라기보다 Swap Chain Back Buffer의 표시 Mode를 설명한다.
		 // 너비, 높이, Refresh Rate, Pixel Format, Scanline 순서와 Scaling 정보를 담으며
		 // CreateSwapChain이 이 정보를 사용해 Buffer를 구성한다.
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
		//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

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
	void Renderer::CreateTansformBuffer()
	{
		// 버퍼 구성 정보
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(Matrix4);				// 버퍼 크기
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// 사용 목적
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA indexBufferData = {};
		indexBufferData.pSysMem = Matrix4::Identity.Data();

		ThrowIfFailed(device->CreateBuffer(&bufferDesc, &indexBufferData, &transformBuffer), L"Failed to create transform buffer");

	}
	void Renderer::UpdateTrnasformBuffer(const Matrix4& worldMatrix)
	{
		// 버퍼에 저장할 데이터 설정 과정 처리
		// context->UpdateSubresource() : 일반 버퍼의 데이터 변경 가능
		// 간할적인(너무 자주 아닌) 데이터 갱신 시 사용 권장

		// 버퍼와 연결할 리소스 생성
		D3D11_MAPPED_SUBRESOURCE mapped = {};

		// 버퍼와 연동

		/*
		* [도와줘요 CODEX]
		 D3D11_MAP_WRITE_DISCARD - 리소스 폐기??
		 mapped에 넣으면 GPU가 transformBuffer에 넣어줌?

		 [CODEX 답변]
		 WRITE_DISCARD는 기존 버퍼 내용을 보존하지 않겠다고 알리는 방식이다.
		 GPU가 이전 내용을 사용하는 중이어도 드라이버가 새 쓰기 영역을 줄 수 있어 CPU 대기를 줄인다.
		 mapped.pData는 Map이 돌려준 CPU 쓰기용 주소이며, 여기에 새 행렬을 복사한 뒤 Unmap하면
		 이후 Draw 명령에서 GPU가 바인딩된 transformBuffer의 새 내용을 읽는다.
		 따라서 GPU가 mapped에 값을 넣는 것이 아니라 CPU가 버퍼에 전달할 값을 작성하는 과정이다.
		*/

		ThrowIfFailed(context->Map(transformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped), L"Failed to map transform buffer");

		// 업데이트 할 데이터 설정
		std::memcpy(mapped.pData, worldMatrix.Data(), sizeof(Matrix4));

		// 버퍼와 연동 해제
		context->Unmap(transformBuffer, 0);
	}
	void Renderer::CreateCameraBuffer()
	{
		// 버퍼 구성 정보
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = sizeof(Matrix4);				// 버퍼 크기
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;				// 사용 목적
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = &cameraData;

		ThrowIfFailed(device->CreateBuffer(&bufferDesc, &bufferData, &cameraBuffer), L"Failed to create camera buffer");
	}

	void Renderer::UpdateCameraBuffer()
	{
		// 버퍼와 연결할 리소스 생성
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		ThrowIfFailed(context->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped), L"Failed to map camera buffer");

		// 업데이트 할 데이터 설정
		std::memcpy(mapped.pData, &cameraData, sizeof(CameraBufferData));

		// 버퍼와 연동 해제
		context->Unmap(cameraBuffer, 0);
	}
}
