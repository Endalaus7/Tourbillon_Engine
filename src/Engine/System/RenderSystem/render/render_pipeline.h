#pragma once
#include <memory>
#include <type_traits>
#include "render_pass.h"
#include <unordered_map>
#include "Structure/TB_List.hpp"
#include "Math/mat44.hpp"
#include "ECSManager.h"
namespace TourBillon
{
	struct RenderPipelineInitInfo{
		std::shared_ptr<RHI> rhi;
		//Ñ¡ÔñÍ¨µÀ
	};
	struct RenderPassInitInfo;
	class RenderPipeline
	{
	public:
		virtual void initialize(RenderPipelineInitInfo init_info);

		virtual void deferredRender(float dt, RHIDrawInfo& drawinfo);

		void passUpdateAfterRecreateSwapchain();

		
		void SetMainCamera(Entity camera);
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

	protected:
		void cacheUniformObject(const TBMath::Mat44& proj_view_matrix);

		void cacheUniformDynamicObject();

	private:

		std::vector<RenderPass*> m_RenderPassList;

		std::shared_ptr<RHI> m_rhi;
	};
}