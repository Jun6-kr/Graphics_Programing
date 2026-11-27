#include "StaticMesh.h"
#include "Renderer.h"
#include "Vertex.h"
#include "AssetManager.h"

#include <Core/Core.h>
#include <cassert>
#include <stdexcept>

namespace Craft
{
	StaticMesh::StaticMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		// 정점 / 인덱스 배열의 항목 수가 0보다 큰지 확인
		assert(vertices.size() > 0);
		assert(indices.size() > 0);

		// 인덱스 개수 설정
		indexCount = static_cast<uint32_t>(indices.size());

		// 정점 데이터 바이트 너비 설정
		stride = sizeof(vertices[0]);

		// Renderer 장치 얻어오기
		auto& device = Renderer::Get().GetDevice();

		// 버퍼 구성 정보
		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.ByteWidth = static_cast<uint32_t>(stride * vertices.size());			// stride(12byte, float 3개) * 그 vetex의 수량
		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 목적
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA vertexBufferData = {};
		vertexBufferData.pSysMem = vertices.data();		// 오호 stl(std::vector)의 기능이군요

		ThrowIfFailed(device.CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer), L"Failed to create vertex buffer");

		// 버퍼 구성 정보
		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * static_cast<uint32_t>(indices.size());			// 버퍼 크기
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 사용 목적
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// 버퍼에 저장할 데이터
		D3D11_SUBRESOURCE_DATA indexBufferData = {};
		indexBufferData.pSysMem = indices.data();


		ThrowIfFailed(device.CreateBuffer(&indexBufferDesc, &indexBufferData, &indexBuffer), L"Failed to create index buffer");
	}

	StaticMesh::~StaticMesh()
	{
		// 리소스 해제
		SafeRelease(vertexBuffer);
		SafeRelease(indexBuffer);
	}

	void StaticMesh::Bind() const
	{
		auto& context = Renderer::Get().GetContext();

		uint32_t offset = 0;
		context.IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context.IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		// 토폴로지 -> 정점을 어떻게 이엉서 도형을 만들지 결정하는 방식
		context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	std::shared_ptr<StaticMesh> StaticMesh::CreateTriangleMesh()
	{
		return std::make_shared<StaticMesh>(
			std::vector<Vertex> { 
			Vertex{ Vector3{ 0.0f, 0.5f, 0.5f }, Vector2(0.5f, 0.0f) }
			, Vertex{ Vector3{ 0.5f, -0.5f, 0.5f }, Vector2(1.0f, 1.0f) }
			, Vertex{ Vector3{ -0.5f, -0.5f, 0.5f }, Vector2(0.0f, 1.0f) }
		}
		, std::vector <uint32_t> {0, 1, 2});	// 왼손 좌표계 순서에 맞기만 하면, 시작 순서는 크게 관련 X
		// [CODEX 답변] 시작 정점 자체보다 Winding Order(시계/반시계 방향)가 중요하다.
		// 같은 방향을 유지하는 0, 1, 2와 1, 2, 0은 같은 면이지만, 순서를 뒤집으면 앞면과 뒷면 판정이 바뀐다.
	}
	std::shared_ptr<StaticMesh> StaticMesh::CreateQuadMesh()
	{
		return std::make_shared<StaticMesh>(
			std::vector<Vertex> {
			Vertex{ Vector3{ -0.5f, 0.5f, 0.5f }, Vector2(0.0f, 0.0f) }
				, Vertex{ Vector3{ 0.5f, 0.5f, 0.5f }, Vector2(1.0f, 0.0f) }
				, Vertex{ Vector3{ 0.5f, -0.5f, 0.5f }, Vector2(1.0f, 1.0f) }
				, Vertex{ Vector3{ -0.5f, -0.5f, 0.5f }, Vector2(0.0f, 1.0f) }
		}
		//, std::vector <uint32_t> {0, 1, 2, 3});
		, std::vector <uint32_t> { 0, 1, 2, 0, 2, 3 });
		/*
			[Codex] 굳이 사각형을 삼각형 2개로 만드는 이유?
			[CODEX 답변]
			Direct3D의 기본 Primitive는 점, 선, 삼각형이며 사각형 Primitive는 따로 없다.
			따라서 Triangle List에서 인덱스 { 0, 1, 2 }와 { 0, 2, 3 }으로 두 삼각형을 만들고,
			두 삼각형이 대각선 정점 0과 2를 공유해 하나의 사각형 영역을 채우도록 구성한다.
		*/
	}

	std::shared_ptr<StaticMesh> StaticMesh::LoadFromFile(const std::string& path)
	{
		// 커스텀 cmesh 애셋 로드
		const auto asset = AssetManager::Get().LoadMesh(path);

		// 로드 실패 처리
		if (nullptr == asset)
		{
			throw std::runtime_error("Failed to load cmesh asset: " + path);
			return {};
		}

		// 정점/인덱스 배열이 제대로 설정됐는지 확인
		if (asset->vertices.empty() || asset->indices.empty())
		{
			throw std::runtime_error("Asset has no vertices");
			return {};
		}

		// 커스텀 애셋 데이터를 엔진의 데이터로 변환
		// 정점 배열 선언
		std::vector<Vertex> vertices;

		// 정점 배열 크기 확보 (재할당 방지)
		vertices.reserve(asset->vertices.size());

		// 애셋에서 엔진의 정점 데이터로 변환 및 배열에 추가
		for (const AssetVertex& sourceVertex : asset->vertices)
		{
			Vertex vertex = {};
			
			// 위치 저장
			vertex.position = { sourceVertex.position.x, sourceVertex.position.y, sourceVertex.position.z };

			// 텍스처 좌표(UV) 저장
			vertex.uv = { sourceVertex.uv.x, sourceVertex.uv.y };

			// 배열에 추가
			vertices.emplace_back(std::move(vertex));
		}

		// 스태틱 메시 생성 후 반환
		return std::make_shared<StaticMesh>(vertices, asset->indices);
	}
}
