#include "Engine.h"
#include "ECSManager.h"
#include "Components/CameraComponent.h"
#include "Components/GeometryComponent.h"
#include "Components/RenderableComponent.h"

#include "RenderSystem/render/render_system.h"
#include "rhi/rhi_source.h"

void TourBillon::TBEngine::initialize(EngineInitInfo engine_init_info)
{
	ECSManagerInitInfo ecs_manager_init_info;
	ECSManager::Instance()->initialize(ecs_manager_init_info);


	ECSManager::Instance()->RegisterComponent<Geometry>();
	ECSManager::Instance()->RegisterComponent<Camera3D>();
	ECSManager::Instance()->RegisterComponent<Transfrom>();

	m_renderSystem = ECSManager::Instance()->mSystemManager->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(ECSManager::Instance()->GetComponentType<Geometry>());
		ECSManager::Instance()->SetSystemSignature<RenderSystem>(signature);
	}
	
	RenderSystemInitInfo render_system_init_info;
	render_system_init_info.rhi_type = VULKAN_RHI;
	render_system_init_info.frame_rate = 60;
	render_system_init_info.window_width = engine_init_info.window_width;
	render_system_init_info.window_height = engine_init_info.window_height;
	m_renderSystem->initialize(&render_system_init_info);


	//相机
	Entity camera_entity = ECSManager::Instance()->CreateEntity();
	Camera3D camera;
	camera.pos = TBMath::Vec3(2,2,-2);
	camera.lookat = TBMath::Vec3(0,0,0);
	camera.up = TBMath::Vec3(0,0,-1);
	camera.isOrthographic = false;
	camera.fovX = 60;
	camera.fovY = ((float)engine_init_info.window_width / (float)engine_init_info.window_width) * camera.fovX;
	camera.nearClip = 0.1f;
	camera.farClip = 10.0f;
	ECSManager::Instance()->AddComponent<Camera3D>(camera_entity, camera);
	m_renderSystem->SetMainCamera(camera_entity);

	
	//手动初始化几何体
	//std::vector<Entity> entities(MAX_ENTITIES - 1);
	int index = 0;
	std::vector<Entity> entities(20);

	std::shared_ptr<Geometry> mesh = std::make_shared<Geometry>();
	Vertex v1(Point3d(-0.5f, -0.5f, 0.f), Point2d(1.0f, 0.0f), ColorRGBA(1.0f, 0.0f, 0.0f, 1.0f));
	Vertex v2(Point3d(0.5f, -0.5f, 0.f), Point2d(1.0f, 0.0f), ColorRGBA(0.0f, 1.0f, 0.0f, 1.0f));
	Vertex v3(Point3d(0.5f, 0.5f, 0.f), Point2d(1.0f, 0.0f), ColorRGBA(0.0f, 0.0f, 1.0f, 1.0f));
	Vertex v4(Point3d(-0.5f, 0.5f, 0.f), Point2d(1.0f, 0.0f), ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
	Index_3P i1(0, 1, 2);
	Index_3P i2(2, 3, 0);
	mesh->vertexArray.push_back(v1);
	mesh->vertexArray.push_back(v2);
	mesh->vertexArray.push_back(v3);
	mesh->vertexArray.push_back(v4);
	mesh->indexArray.push_back(i1);
	mesh->indexArray.push_back(i2);
	mesh->vertexBuffer = new RHIBufferResource;
	mesh->indexBuffer = new RHIBufferResource;
	m_renderSystem->loadMeshBuffer(*mesh);
	Entity mesh_entity = ECSManager::Instance()->CreateEntity();
	ECSManager::Instance()->AddComponent<Geometry>(mesh_entity, *mesh);
	

	for (auto& entity : entities)
	{
		entity = ECSManager::Instance()->CreateEntity();

		Transfrom trans;
		trans.rotation = TBMath::Vec3(0, 0, 0);
		trans.scale = TBMath::Vec3(1, 1, 1);
		trans.translation = TBMath::Vec3(1.5f * (index - 4), 0, 0);

		Renderable renderObj;
		renderObj.mesh = mesh;

		ECSManager::Instance()->AddComponent<Renderable>(entity, renderObj);
		ECSManager::Instance()->AddComponent<Transfrom>(entity, trans);
		index++;
	}
}

void TourBillon::TBEngine::run()
{
	ECSManager::Instance()->run();
	std::function<void(float)> boundBeforeRender = std::bind(&TBEngine::UpdateBeforeRender, this, std::placeholders::_1);
	std::function<void(float)> boundAfterRender = std::bind(&TBEngine::UpdateAfterRender, this, std::placeholders::_1);

	m_renderSystem->rendLoop(boundBeforeRender, boundAfterRender);//PollEvents必须在主线程中运行
}


void TourBillon::TBEngine::UpdateBeforeRender(float dt)
{
	static float rotateCamera = 0;
	rotateCamera += dt * 0.2;
	const auto& trans_components = ECSManager::Instance()->GetComponentEntities<Camera3D>();
	for (auto entity : trans_components)
	{
		auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(entity);
		camera.pos.x -= dt * 0.1;
	}
	return;
}

void TourBillon::TBEngine::UpdateAfterRender(float dt)
{
	return;
}
