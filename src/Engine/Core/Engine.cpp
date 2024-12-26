#include "Engine.h"
#include "ECSManager.h"
#include "Components/CameraComponent.h"
#include "Components/GeometryComponent.h"
#include "Components/KeysComponent.h"
#include "Components/WindowComponent.h"
#include "Components/MaterialComponent.h"

#include "RenderSystem/render_system.h"
#include "IOSystem/IOSystem.h"
#include "rhi/rhi_source.h"

void TourBillon::TBEngine::initialize(EngineInitInfo engine_init_info)
{
	ECSManagerInitInfo ecs_manager_init_info;
	ECSManager::Instance()->initialize(ecs_manager_init_info);


	ECSManager::Instance()->RegisterComponent<RenderWindow>();
	ECSManager::Instance()->RegisterComponent<GeometryShared>();
	ECSManager::Instance()->RegisterComponent<Camera3D>();
	//ECSManager::Instance()->RegisterComponent<Material>();
	ECSManager::Instance()->RegisterComponent<Transfrom>();
	ECSManager::Instance()->RegisterComponent<Buttons>();
	ECSManager::Instance()->RegisterComponent<Mouse>();

	m_renderSystem = ECSManager::Instance()->mSystemManager->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		//signature.set(ECSManager::Instance()->GetComponentType<Renderable>());
		ECSManager::Instance()->SetSystemSignature<RenderSystem>(signature);
	}

	RenderSystemInitInfo render_system_init_info;
	render_system_init_info.rhi_type = VULKAN_RHI;
	render_system_init_info.frame_rate = 60;
	render_system_init_info.window_width = engine_init_info.window_width;
	render_system_init_info.window_height = engine_init_info.window_height;
	render_system_init_info.window_num = engine_init_info.window_num;
	m_renderSystem->initialize(&render_system_init_info);

	//最后一个初始化
	auto& io_system = ECSManager::Instance()->mSystemManager->RegisterSystem<IOSystem>();
	{
		Signature signature;
		//signature.set(ECSManager::Instance()->GetComponentType<Renderable>());
		ECSManager::Instance()->SetSystemSignature<IOSystem>(signature);
	}
	IOSystemInitInfo io_system_init_info;
	io_system->initialize(&io_system_init_info);

	//相机
	for(int windowindex = 0;windowindex< engine_init_info.window_num;windowindex++)
	{
		Entity camera_entity = ECSManager::Instance()->CreateEntity();
		Camera3D camera0;
		camera0.pos = TBMath::Vec3(2, 2, -2);
		camera0.lookat = TBMath::Vec3(0, 0, 0);
		camera0.up = TBMath::Vec3(0, 0, -1);
		camera0.isOrthographic = false;
		camera0.fovX = 60;
		camera0.fovY = ((float)engine_init_info.window_width / (float)engine_init_info.window_width) * camera0.fovX;
		camera0.nearClip = 0.1f;
		camera0.farClip = 100.0f;
		ECSManager::Instance()->AddComponent<Camera3D>(camera_entity, camera0);
		m_renderSystem->SetMainCamera(windowindex, camera_entity);
	}

	//手动初始化几何体
	//std::vector<Entity> entities(MAX_ENTITIES - 1);
	int index = 0;
	std::vector<Entity> entities(8);

	Geometry* mesh = new Geometry;
	Vertex v1(Point3d(-0.5f, -0.5f, 0.f), Point2d(0.0f, 0.0f), ColorRGBA(1.0f, 0.0f, 0.0f, 1.0f));
	Vertex v2(Point3d(0.5f, -0.5f, 0.f), Point2d(1.0f, 0.0f), ColorRGBA(0.0f, 1.0f, 0.0f, 1.0f));
	Vertex v3(Point3d(0.5f, 0.5f, 0.f), Point2d(1.0f, 1.0f), ColorRGBA(0.0f, 0.0f, 1.0f, 1.0f));
	Vertex v4(Point3d(-0.5f, 0.5f, 0.f), Point2d(0.f, 1.0f), ColorRGBA(0.0f, 0.0f, 0.0f, 1.0f));
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
	//Entity mesh_entity = ECSManager::Instance()->CreateEntity();
	//ECSManager::Instance()->AddComponent<Geometry>(mesh_entity, *mesh);
	
	
	for (auto& entity : entities)
	{
		entity = ECSManager::Instance()->CreateEntity();

		Transfrom trans;
		trans.rotation = TBMath::Vec3(0, 0, 0);
		trans.scale = TBMath::Vec3(1, 1, 1);
		trans.translation = TBMath::Vec3(1.1f * (index - 2), 0, 0);

		GeometryShared meshptr;
		meshptr.setAssetData("Plane", mesh);

		ECSManager::Instance()->AddComponent<Transfrom>(entity, trans);
		ECSManager::Instance()->AddComponent<GeometryShared>(entity, meshptr);
		index++;
	}
}

void TourBillon::TBEngine::run()
{
	m_renderSystem->rendLoop(METHOD_LISTENER(TBEngine::UpdateBeforeRender), METHOD_LISTENER(TBEngine::UpdateAfterRender));//PollEvents必须在主线程中运行
}


void TourBillon::TBEngine::UpdateBeforeRender(float dt)
{
	static float rotateCamera = 0;
	rotateCamera += dt * 0.2;
	//const auto& camera_components = ECSManager::Instance()->GetComponentEntities<Camera3D>();
	//for (auto entity : camera_components)
	//{
	//	auto& camera = ECSManager::Instance()->GetComponent<Camera3D>(entity);
	//	if(entity % 2)
	//		camera.pos.x -= dt * 0.1;
	//	else
	//		camera.pos.x += dt * 0.1;
	//
	//}
	return;
}

void TourBillon::TBEngine::UpdateAfterRender(float dt)
{
	return;
}
