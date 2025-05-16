#pragma once
#include "render/render_pass.h"
#include "Math/mat44.hpp"
#include "ECSManager.h"

//一般的pass模板，由.shader文件导入


namespace TourBillon
{
	struct RenderPassInitInfo;
	struct GeneralPassInitInfo :public RenderPassInitInfo
	{

	};

	class GeneralPass :public RenderPass
	{
	public:
		~GeneralPass();
		virtual void initialize(const RenderPassInitInfo* init_info);
		virtual void postInitialize();

		virtual void beforeDraw(float dt, RHIDrawInfo& drawinfo)override;
		virtual void drawPass(float dt, RHIDrawInfo& drawinfo)override;

		virtual void setup_DescriptorSetLayout()override;
		virtual void setup_RenderPass()override;
		virtual void setup_Pipeline()override;
		//virtual void setup_FrameBuffer()override;

		virtual void destroyFramebuffer()override;

		virtual void updateDescriptorSets(float dt, RHIDrawInfo& drawinfo)override;

		void setMainCamera(uint32_t windowindex, Entity camera);



		struct UniformBufferObject
		{
			TBMath::Mat44 proj_view_matrix;
		};
		struct alignas(256) UniformBufferDynamicObject
		{
			TBMath::Mat44 model_matrix;
			//Vector4 color;
		};

		//struct Descriptor
		//{
		//	RHIDescriptorSetLayout* layout = nullptr;
		//	std::vector<RHIDescriptorSet*> descriptor_sets;
		//};

		void updateUboData();
		void updateUniformUboData(uint32_t windowindex);
		void updateUboBuffer(RHIDrawInfo& info);
		void updateMVPDescriptorSet();
		void updateImageDescriptorSet();

		void dirtyUniformBuffer();
	private:
		void cacheUniformObject(const TBMath::Mat44& proj_view_matrix);
		void cacheUniformDynamicObject();

		void setupDescriptorSet();

		//Descriptor m_descriptor;
		std::vector<Entity> m_camera;

		uint32_t m_renderCount = 0;//render entities number

		RHIBufferResource* m_uniformbuffer;//通用uniform
		UniformBufferObject m_uniform_buffer_object;//vp矩阵

		RHIBufferResource* m_uniformdynamicbuffer;//动态uniform
		std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;

	};
}