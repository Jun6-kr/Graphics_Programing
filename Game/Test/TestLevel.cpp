#include "TestLevel.h"
#include "TestActor.h"
#include <Component/TransformComponent.h>
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
	auto shader = std::make_shared<Shader>(L"HLSLShaders/DefaultVS.hlsl", L"HLSLShaders/DefaultPS.hlsl");

	// 액터 생성
	auto left = SpawnActor<TestActor>();
	left->speed = 60.f;
	left->GetTransform().scale = Vector3::One * 0.4f;
	left->GetTransform().position.x = -0.5f;
	left->SetStaticMesh(triangle);
	left->SetShader(shader);


	auto right = SpawnActor<TestActor>();
	right->speed = 600.f;
	right->GetTransform().scale = Vector3::One * 0.4f;
	right->GetTransform().position.x = 0.5f;
	right->SetStaticMesh(quad);
	right->SetShader(shader);

	auto center = SpawnActor<TestActor>();
	center->GetTransform().scale = Vector3::One * 0.4f;
	center->SetStaticMesh(quad);
	center->SetShader(shader);
}
