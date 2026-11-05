#include "TestLevel.h"
#include "TestActor.h"

#include <Actor/CameraActor.h>
#include <Component/StaticMeshComponent.h>
#include <Component/TransformComponent.h>
#include <Component/CameraMovementComponent.h>

#include <Graphics/Material.h>
#include <Graphics/StaticMesh.h>
#include <Graphics/ShaderManager.h>
#include <Graphics/TextureManager.h>

using namespace Craft;

void TestLevel::Initialized()
{
	Level::Initialized();

	//auto texture = Texture::Load("Assets/T_Checker.png");
	//auto texture = TextureManager::Get().LoadTexture("Assets/T_Checker.png");

	// 리소스 생성
	auto triangle = StaticMesh::CreateTriangleMesh();
	auto quad = StaticMesh::CreateQuadMesh();

	// 하나의 셰이더를 생성해서 노나쓴다.
	//auto defaultShader = Shader::CreateDefaultShader();
	//auto yellowShader = std::make_shared<Shader>(L"Yellow");
	auto defaultShader = ShaderManager::Get().LoadShader(L"Default");
	auto yellowShader = ShaderManager::Get().LoadShader(L"TextureMapping");

	auto defaultMaterial = std::make_shared<DefaultMaterial>();
	auto textureMappingMaterial = std::make_shared<TextureMappingMaterial>("Assets/T_Checker.png");

	// 액터 생성
	auto left = SpawnActor<TestActor>();
	left->speed = 60.f;
	left->GetTransform().scale = Vector3::One * 0.4f;
	left->GetTransform().position.x = -0.5f;

	// 메시 설정
	left->GetComponent<StaticMeshComponent>()->SetStaticMesh(triangle);
	//left->GetComponent<StaticMeshComponent>()->SetShader(defaultShader);
	left->GetComponent<StaticMeshComponent>()->SetMaterial(defaultMaterial);


	auto right = SpawnActor<TestActor>();
	right->speed = 600.f;
	right->GetTransform().scale = Vector3::One * 0.4f;
	right->GetTransform().position.x = 0.5f;
	//right->GetTransform().position.z = 1.5f;
	right->GetComponent<StaticMeshComponent>()->SetStaticMesh(quad);
	right->GetComponent<StaticMeshComponent>()->SetMaterial(textureMappingMaterial);
	//right->GetComponent<StaticMeshComponent>()->SetShader(yellowShader);
	//right->GetComponent<StaticMeshComponent>()->SetTexture(texture);

	auto center = SpawnActor<TestActor>();
	center->GetTransform().scale = Vector3::One * 0.4f;
	center->speed = 10.f;

	auto centerMeshComponent = center->GetComponent<StaticMeshComponent>();
	centerMeshComponent->SetStaticMesh(quad);
	//rightMeshComponent->SetShader(yellowShader);
	//rightMeshComponent->SetTexture(texture);
	centerMeshComponent->SetMaterial(textureMappingMaterial);

	auto camera = SpawnActor<CameraActor>();
	camera->AddComponent<CameraMovementComponent>();
	camera->GetTransform().position.z = -3.0f;
}
