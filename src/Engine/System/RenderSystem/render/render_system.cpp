#include "render_system.h"

#include "rhi/rhi.h"
#include "rhi/rhi_window.h"
#include "render/render_pipeline.h"
#include "event.hpp"

#include "ECSManager.h"

#include <chrono>


TourBillon::RenderSystem::~RenderSystem()
{
    //need vk destroy
    
}

void TourBillon::RenderSystem::initialize(SystemInitInfo* init_info)
{
    RenderSystemInitInfo* render_init_info = dynamic_cast<RenderSystemInitInfo*>(init_info);
    if (!render_init_info)
        return;

    WindowCreateInfo window_create_info;
    window_create_info.width = render_init_info->window_width;
    window_create_info.height = render_init_info->window_height;
    window_create_info.title = "render window";

    m_rhiWindows.resize(2);

    for(auto& rhiwindow: m_rhiWindows)
    {
        rhiwindow = RHI_Factory::Instance()->createRHIWindow(render_init_info->rhi_type);
        rhiwindow->initialize(window_create_info);
    }

    m_frame_rate = render_init_info->frame_rate;


    RHIInitInfo rhi_init_info;
    rhi_init_info.window_systems = m_rhiWindows;
    m_rhi = RHI_Factory::Instance()->createRHI(render_init_info->rhi_type);
    m_rhi->initialize(rhi_init_info);

    RenderPipelineInitInfo render_pipeline_init_info;
    render_pipeline_init_info.rhi = m_rhi;
    m_renderPipeline = std::make_shared<RenderPipeline>();
    m_renderPipeline->initialize(render_pipeline_init_info);

}

void TourBillon::RenderSystem::rendLoop(std::function<void(float)> beforeRender, std::function<void(float)> afterRender)
{
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    while (!shouldClose())
    {
        
        // 获取当前时间
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();

        // 计算当前帧与上一帧之间的时间差 (delta time)
        std::chrono::duration<float> deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - lastTime);
        lastTime = currentTime;

        beforeRender(deltaTime.count());

        for (auto rhiwindow : m_rhiWindows)
            rhiwindow->pollEvents();
        //m_rhi->UpdateDraw(deltaTime.count());

		RHIDrawInfo drawinfo;

        //此处需要修改
        //每种mesh只需要在meshinfo中添加一次
        const auto& geo_components = ECSManager::Instance()->GetComponentEntities<Geometry>();
        drawinfo.drawMeshinfos.resize(geo_components.size());
        uint32_t index = 0;
        for (auto& entity : geo_components)
        {
            auto& geometry = ECSManager::Instance()->GetComponent<Geometry>(entity);

            drawinfo.drawMeshinfos[index].vertex_buffer = geometry.vertexBuffer;
            drawinfo.drawMeshinfos[index].index_buffer = geometry.indexBuffer;
            drawinfo.drawMeshinfos[index].indices_count = geometry.indexArray.size() * 3;

            index++;
            //m_rhi->DrawMesh(draw_mesh_info);
            //m_rhi->DrawDebug();
        }

        //drawinfo.drawEvents.addCallback([&](const CEvent&) {
        //    
        //    }
        //);
        
        float dt = deltaTime.count();
        //多窗口渲染
        m_renderPipeline->BeforeFrameDraw(dt, drawinfo);
        for(int winindex=0; winindex <m_rhiWindows.size(); winindex++)
        {
            //drawinfo.windowIndex = winindex;
            drawinfo.windowIndex = winindex;
            m_renderPipeline->deferredRender(dt, drawinfo);
        }
        m_renderPipeline->AfterFrameDraw(dt, drawinfo);
        afterRender(dt);

        //LOG_DEBUG(std::to_string(1.f / deltaTime.count()));
        //m_rhi->waitFrameTime(1.f / m_frame_rate);
    }
}

void TourBillon::RenderSystem::clear()
{
    m_rhi.reset();
    for(auto rhiwindow: m_rhiWindows)
        rhiwindow.reset();
    clearBuffers();
    m_vertex_cache.clear();
}

void TourBillon::RenderSystem::loadMeshBuffer(Geometry& mesh)
{
    uint64_t vertexbufferSize = static_cast<uint64_t>(mesh.vertexArray.size() * sizeof(Vertex));
    m_rhi->createVertexBuffer((void*)mesh.vertexArray.data(), vertexbufferSize, mesh.vertexBuffer->buffer, mesh.vertexBuffer->buffermemory);

    uint64_t indexbufferSize = static_cast<uint64_t>(mesh.indexArray.size() * sizeof(Index_3P));
    m_rhi->createIndexBuffer((void*)mesh.indexArray.data(), indexbufferSize, mesh.indexBuffer->buffer, mesh.indexBuffer->buffermemory);
}

void TourBillon::RenderSystem::SetMainCamera(Entity camera)
{
    m_renderPipeline->SetMainCamera(camera);
}

void TourBillon::RenderSystem::clearBuffers()
{
	RHIResourceManager::Instance()->Release(m_vertexbuffer, 5);
	m_vertexbuffer = nullptr;
}

bool TourBillon::RenderSystem::shouldClose()
{
    for (auto rhiwindow : m_rhiWindows)
    {
        if (!rhiwindow->shouldClose())
            return false;
    }
    return true;
}
