#include "TestLevel.h"
#include "TestActor.h"
#include <Component/CameraComponent.h>
#include <Component/StaticMeshComponent.h>
#include <Component/TransformComponent.h>
#include <Graphics/Shader.h>
#include <Graphics/StaticMesh.h>
#include <memory>

#include <Graphics/Texture.h>

using namespace Craft;

void TestLevel::Initialized()
{
	Level::Initialized();

	auto texture = Texture::Load("Assets/T_Checker.jpg");

	// 리소스 생성
	auto triangle = StaticMesh::CreateTriangleMesh();
	auto quad = StaticMesh::CreateQuadMesh();

	// 하나의 셰이더를 생성해서 노나쓴다.
	auto defaultShader = Shader::CreateDefaultShader();
	auto yellowShader = std::make_shared<Shader>(L"Yellow");


	// 액터 생성
	auto left = SpawnActor<TestActor>();
	left->speed = 60.f;
	left->GetTransform().scale = Vector3::One * 0.4f;
	//left->GetTransform().position.x = -0.5f;

	// 메시 설정
	left->GetComponent<StaticMeshComponent>()->SetStaticMesh(triangle);
	left->GetComponent<StaticMeshComponent>()->SetShader(defaultShader);


	auto right = SpawnActor<TestActor>();
	right->speed = 600.f;
	right->GetTransform().scale = Vector3::One * 0.4f;
	//right->GetTransform().position.x = 0.5f;
	right->GetTransform().position.z = 1.5f;
	right->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
	right->GetComponent<StaticMeshComponent>()->SetShader(defaultShader);

	auto center = SpawnActor<TestActor>();
	center->GetTransform().scale = Vector3::One * 0.4f;
	center->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
	center->GetComponent<StaticMeshComponent>()->SetShader(yellowShader);

	auto camera = SpawnActor<TestActor>();
	camera->AddComponent<CameraComponent>();
	camera->GetTransform().position.z = -3.0f;
}
