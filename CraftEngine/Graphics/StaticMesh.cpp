#include "StaticMesh.h"
#include "Renderer.h"
#include <Core/Core.h>
#include <cassert>

namespace Craft
{
	StaticMesh::StaticMesh(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices)
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
			std::vector<Vector3> { Vector3{ 0.0f, 0.5f, 0.5f }, Vector3{ 0.5f, -0.5f, 0.5f }, Vector3{ -0.5f, -0.5f, 0.5f }}
		, std::vector <uint32_t> {0, 1, 2});
	}
}
