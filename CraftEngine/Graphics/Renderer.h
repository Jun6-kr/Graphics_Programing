#pragma once
#include <Core/Core.h>
// direct3d 라이브러리의 객체를 생성하기 위해 추가
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>

namespace Craft
{
	// 전방 선언
	class Win32Window;

	// CPU에서 GPU로 명령을 전달
	class /*CRAFT_API*/ Renderer
	{
	public:
		Renderer(const Win32Window& window);
		~Renderer();

		void Draw(float red, float green, float blue, uint32_t vsync);

		// 크기 변경 이벤트 함수
		void OnResize(uint32_t width, uint32_t height);

	private:
		// 그리기 단계 별로 실행되는 함수

		// 그리기 준비
		void BeginScene(float red, float green, float blue);
		// 장면 그리기 - Draw Call 발생
		void DrawScene();
		// 버퍼 교화
		void EndScene(uint32_t vsync);

		// 장치 생성
		void CreateDevices();

		// 스왑체인 생성 - GPU가 그린 화면을 실제 Windows 창에 보여주기 위해 화면 버퍼를 관리하는 객체
		// -> Direct3D와 창(window)를 연계해주는 역할
		void CreateSwapChain(const Win32Window& window);

		// 렌더 타켓 뷰 생성
		void CreateRenderTargetView();

		// 데모를 위한 버퍼 생성 함수
		// 버퍼 = 데이터 전송에 사용되는 메모리 덩어리
		void CreateDemoBuffers();

		// 기본 셰이더 생성 함수
		void CreateDefaultShaders();

		void CreateViewport(uint32_t width, uint32_t height);

	private:
		// 장치(그래픽카드)

		// 디바이스 -> 데이터 생성
		// GPU에서 사용할, 리소스  제작 담당
		ID3D11Device* device = nullptr;

		// 그래픽카드에 데이터 설정(연결/바인딩)
		// device에 의해 만들어진 리소스를 설정하고 명령하는 개체
		ID3D11DeviceContext* context = nullptr;

		// 기능 변경이 거의 없는 장치
		IDXGISwapChain* swapChain = nullptr;

		// 백버퍼를 대표하는 렌더 타겟 - 그래픽 카드에 그릴 대상을 선정할 떄 사용하는 타입
		ID3D11RenderTargetView* renderTargetView = nullptr;

		// 정점 버퍼(VertextBuffer)
		ID3D11Buffer* vertexBuffer = nullptr;

		// 인덱스(색인) 버퍼
		ID3D11Buffer* indexBuffer = nullptr;

		// 셰이더 관련 변수
		ID3D11VertexShader* vertexShader = nullptr;
		ID3D11PixelShader* pixelShader = nullptr;

		// 입력의 정보를 전달하는데 사용되는 객체
		ID3D11InputLayout* inputLayout = nullptr;

		// 뷰포트 - 화면을 어떻게 사용할지 정의하는 구조체
		D3D11_VIEWPORT viewport = {};
	};
}