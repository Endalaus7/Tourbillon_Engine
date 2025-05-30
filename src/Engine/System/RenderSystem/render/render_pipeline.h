#pragma once
#include <memory>
#include <type_traits>
#include "render_pass.h"
#include <unordered_map>
#include "Structure/TB_List.hpp"
#include "Math/mat44.hpp"
#include "ECSManager.h"

//渲染总管线
//管线应当可以在c++自定义，继承此类后在rendersystem加入

namespace TourBillon
{
	struct RenderPipelineInitInfo{
		std::shared_ptr<RHI> rhi;
		//选择通道
	};
	struct RenderPassInitInfo;
	class RenderPipelineBase
	{
	public:
		virtual void initialize(RenderPipelineInitInfo init_info);

		virtual void BeforeFrameDraw(float dt, RHIDrawInfo& drawinfo);
		virtual void deferredRender(float dt, RHIDrawInfo& drawinfo);
		virtual void AfterFrameDraw(float dt, RHIDrawInfo& drawinfo);

		void passUpdateAfterRecreateSwapchain();

		
		void SetMainCamera(uint32_t windowindex, Entity camera);
		//template<typename T>
		//std::shared_ptr<T> AddRenderPass(RenderPassInitInfo* render_pass_init_info)
		//{
		//	if (!std::is_base_of<RenderPass, T>::value 
		//		|| !render_pass_init_info)
		//		return nullptr;
		//
		//	auto new_renderpass = std::make_shared<T>();
		//
		//}

		enum PassType {
			Pass_MainCamera,
			Pass_Count
		};

	private:

		std::vector<RenderPass*> m_RenderPassList;

		std::shared_ptr<RHI> m_rhi;
	};
}