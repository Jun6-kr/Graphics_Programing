#include "TestLevel.h"
#include "TestActor.h"
#include <Component/TransformComponent.h>
#include <Component/StaticMeshComponent.h>
#include <Graphics/Shader.h>
#include <Graphics/StaticMesh.h>
#include <memory>

using namespace Craft;

void TestLevel::Initialized()
{
	Level::Initialized();

	// 리소스 생성
	auto triangle = StaticMesh::CreateTriangleMesh();
	auto quad = StaticMesh::CreateQuadMesh();

	// 하나의 셰이더를 생성해서 노나쓴다.
	auto shader = Shader::CreateDefaultShader();

	// 액터 생성
	auto left = SpawnActor<TestActor>();
	left->speed = 60.f;
	left->GetTransform().scale = Vector3::One * 0.4f;
	left->GetTransform().position.x = -0.5f;

	// 메시 설정
	left->GetComponent<StaticMeshComponent>()->SetStaticMesh(triangle);
	left->GetComponent<StaticMeshComponent>()->SetShader(shader);


	auto right = SpawnActor<TestActor>();
	right->speed = 600.f;
	right->GetTransform().scale = Vector3::One * 0.4f;
	right->GetTransform().position.x = 0.5f;
	right->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
	right->GetComponent<StaticMeshComponent>()->SetShader(shader);

	auto center = SpawnActor<TestActor>();
	center->GetTransform().scale = Vector3::One * 0.4f;
	center->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
	center->GetComponent<StaticMeshComponent>()->SetShader(shader);
}
