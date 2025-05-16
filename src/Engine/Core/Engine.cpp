#include "Engine.h"
#include "ECSManager.h"
#include "Components/CameraComponent.h"
#include "Components/GeometryComponent.h"
#include "Components/KeysComponent.h"
#include "Components/WindowComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/RenderComponent.h"

#include "RenderSystem/render_system.h"
#include "CustomWindowSystem/CustomWindowSystem.h"
#include "IOSystem/IOSystem.h"
#include "rhi/rhi_source.h"

void TourBillon::TBEngine::initialize(EngineInitInfo engine_init_info)
{
	ECSManagerInitInfo ecs_manager_init_info;
	ECSManager::Instance()->initialize(ecs_manager_init_info);

	//注册基础组件
	//ECSManager::Instance()->RegisterComponent<RenderWindow>();
	ECSManager::Instance()->RegisterComponent<GeometryPtr>();
	ECSManager::Instance()->RegisterComponent<Camera3D>();
	ECSManager::Instance()->RegisterComponent<Material>();
	ECSManager::Instance()->RegisterComponent<Transfrom>();
	ECSManager::Instance()->RegisterComponent<Buttons>();
	ECSManager::Instance()->RegisterComponent<Mouse>();
	ECSManager::Instance()->RegisterComponent<ECSWindow>();
	ECSManager::Instance()->RegisterComponent<RenderPort>();

	//渲染系统
	m_renderSystem = ECSManager::Instance()->mSystemManager->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		//signature.set(ECSManager::Instance()->GetComponentType<Renderable>());
		ECSManager::Instance()->SetSystemSignature<RenderSystem>(signature);
	}
	RenderSystemInitInfo render_system_init_info;
	render_system_init_info.rhi_type = VULKAN_RHI;
	render_system_init_info.frame_rate = 60;
	//render_system_init_info.window_width = engine_init_info.window_width;
	//render_system_init_info.window_height = engine_init_info.window_height;
	//render_system_init_info.window_num = engine_init_info.window_num;
	m_renderSystem->initialize(&render_system_init_info);

	//IO系统
	auto& io_system = ECSManager::Instance()->mSystemManager->RegisterSystem<IOSystem>();
	{
		Signature signature;
		//signature.set(ECSManager::Instance()->GetComponentType<Renderable>());
		ECSManager::Instance()->SetSystemSignature<IOSystem>(signature);
	}
	IOSystemInitInfo io_system_init_info;
	io_system->initialize(&io_system_init_info);

	auto& custom_window_system = ECSManager::Instance()->mSystemManager->RegisterSystem<CustomWindowSystem>();
	{
		Signature signature;
		//signature.set(ECSManager::Instance()->GetComponentType<Renderable>());
		ECSManager::Instance()->SetSystemSignature<IOSystem>(signature);
	}
	CustomWindowSystemInitInfo custom_window_system_init_info;
	custom_window_system->initialize(&custom_window_system_init_info);
	//模拟脚本的过程，后续读文件

	//窗口
	//窗口仍然需要传递指针给渲染系统记录，用于glfwPollEvents
	Entity window_entity = ECSManager::Instance()->CreateEntity();
	ECSWindow ecswindow;
	ecswindow.width = 500;
	ecswindow.height = 500;
	//ecswindow.index = m_index;
	ecswindow.title = "Renderwindow";
	ecswindow.isFullscreen = false;
	ECSManager::Instance()->AddComponent<ECSWindow>(window_entity, ecswindow);


	

	//相机
	Entity camera_entity = ECSManager::Instance()->CreateEntity();

	//视口
	Entity renderport_entity = ECSManager::Instance()->CreateEntity();
	RenderPort renderport;
	renderport.camera = camera_entity;
	renderport.setParent(window_entity);
	ECSManager::Instance()->AddComponent<RenderPort>(renderport_entity, renderport);


	Transfrom trans;
	trans.position = TBMath::Vec3(6,1.5,-1.5);
	trans.rotation = TBMath::Vec3(20, -50, 0);

	Camera3D camera0;
	//camera0.pos = TBMath::Vec3(2, 2, -2);
	//camera0.lookat = TBMath::Vec3(0, 0, 0);
	//camera0.up = TBMath::Vec3(0, 0, -1);
	camera0.isOrthographic = false;
	camera0.fovX = 60;
	camera0.fovY = 60;// ((float)engine_init_info.window_width / (float)engine_init_info.window_width)* camera0.fovX;
	camera0.nearClip = 0.1f;
	camera0.farClip = 100.0f;
	ECSManager::Instance()->AddComponent<Camera3D>(camera_entity, camera0);
	ECSManager::Instance()->AddComponent<Transfrom>(camera_entity, trans);
	//m_renderSystem->SetMainCamera(windowindex, camera_entity);
	

	

	//手动初始化几何体
	//std::vector<Entity> entities(MAX_ENTITIES - 1);
	int index = 0;
	std::vector<Entity> entities(8);

	GeometryData* mesh = new GeometryData;
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
		trans.position = TBMath::Vec3(1.1f * (index - 2), 0, 0);

		GeometryPtr meshptr;
		meshptr.setDeleteType(AssetsInfo::Delay_Render_Delete);
		meshptr.setDelayFrame(3);
		meshptr.setAssetData(mesh);

		Material material;
		material.MainTexture.setAssetPath(std::string(DATA_DIR) + "/test_1.png");
		
		ECSManager::Instance()->AddComponent<Material>(entity, material);
		ECSManager::Instance()->AddComponent<Transfrom>(entity, trans);
		ECSManager::Instance()->AddComponent<GeometryPtr>(entity, meshptr);
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
