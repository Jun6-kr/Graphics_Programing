#pragma once
#include <Core/Core.h>
// direct3d 라이브러리의 객체를 생성하기 위해 추가
#include <Math/Matrix4.h>
#include <vector>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdint>
#include <memory>

namespace Craft
{
	// 전방 선언
	class Win32Window;
	class StaticMesh;
	class Shader;
	class Texture;

	// CPU에서 GPU로 명령을 전달
	class /*CRAFT_API*/ Renderer
	{
		// 그리기에 필요한 데이터
		struct RenderCommand
		{
			// 월드 행렬
			Matrix4 worldMatrix = Matrix4::Identity;

			// 스태틱 메시
			std::shared_ptr<StaticMesh> mesh;

			// 셰이더
			std::shared_ptr<Shader> shader;

			// 텍스처
			std::shared_ptr<Texture> texture;
		};

		// 카메라 데이터
		struct CameraBufferData
		{
			// 뷰 변환 행렬
			Matrix4 viewMatrix = Matrix4::Identity;

			// 투영 변환 행렬
			Matrix4 projectionMatrix = Matrix4::Identity;
		};

	public:
		Renderer(const Win32Window& window);
		~Renderer();

		void Draw(float red, float green, float blue, uint32_t vsync);

		// 그리기에 필요한 데이터 제출 함수
		void Submit(const std::shared_ptr<StaticMesh>& staticMesh
			, const std::shared_ptr<Shader>& shader
			, const std::shared_ptr<Texture>& texture
			,const Matrix4& worldMatrix);

		// 카메라 행렬 데이터 제출 함수
		void SubmitCamera(const Matrix4& viewMatrix, const Matrix4& projectionMatrix);

		// 크기 변경 이벤트 함수
		void OnResize(uint32_t width, uint32_t height);


		// [codex] 버퍼 생성 요청을 보내 Renderer가 버퍼를 생성해서 포인터를 반환하는 것이 올바른 설계
		// 전역 접근 함수
		static Renderer& Get();

		// Getter
		ID3D11Device& GetDevice() const { return *device; }
		ID3D11DeviceContext& GetContext() const { return *context; }

	private:
		// 그리기 단계 별로 실행되는 함수

		// 그리기 준비
		void BeginScene(float red, float green, float blue);
		// 장면 그리기 - Draw Call 발생
		void DrawScene();

		// 렌더 목록 처리 함수
		void DrawCommand(const RenderCommand& command);

		// 버퍼 교화
		void EndScene(uint32_t vsync);

		// 장치 생성
		void CreateDevices();

		// 스왑체인 생성 - GPU가 그린 화면을 실제 Windows 창에 보여주기 위해 화면 버퍼를 관리하는 객체
		// -> Direct3D와 창(window)를 연계해주는 역할
		void CreateSwapChain(const Win32Window& window);

		// 렌더 타켓 뷰 생성
		void CreateRenderTargetView();

		// 뎁스 스텐실 뷰 생성
		void CreateDepthStencilView(uint32_t width, uint32_t height);

		void CreateViewport(uint32_t width, uint32_t height);

		// 트랜스폼 상수 버퍼 생성 및 갱신 함수
		void CreateTansformBuffer();
		void UpdateTrnasformBuffer(const Matrix4& worldMatrix);

		// 카메라 상수 버퍼 생성 및 갱신 함수
		void CreateCameraBuffer();
		void UpdateCameraBuffer();

	private:
		// 전역 접근 가능하도록 static 변수 선언
		inline static Renderer* instance = nullptr;

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

		// 깊이 처리를 위한 깊이 버퍼 (공간활용을 위해 stencil도 같이 저장)
		ID3D11Texture2D* depthStencilBuffer = nullptr;

		// 깊이 버퍼를 그래픽카드에 전달하는 깊이 뷰
		ID3D11DepthStencilView* depthStencilView = nullptr;

		// 트랜스폼 데이터를 전달할 사우 버퍼
		ID3D11Buffer* transformBuffer = nullptr;

		// 카메라 버퍼 데이터
		CameraBufferData cameraData;

		// 카메라 버퍼 데이터를 전달할 상수 버퍼
		ID3D11Buffer* cameraBuffer = nullptr;

		// 뷰포트 - 화면을 어떻게 사용할지 정의하는 구조체
		D3D11_VIEWPORT viewport = {};

		// 렌더 명령 목록
		std::vector<RenderCommand> renderCommandList;
	};
}
