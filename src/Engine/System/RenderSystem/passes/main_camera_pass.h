#pragma once
#include "render/render_pass.h"
#include "Math/mat44.hpp"
#include "ECSManager.h"
namespace TourBillon
{
	struct RenderPassInitInfo;
	struct MainCameraPassInitInfo :public RenderPassInitInfo
	{

	};

	class MainCameraPass :public RenderPass
	{
	public:
		~MainCameraPass();
		virtual void initialize(const RenderPassInitInfo* init_info);
		virtual void postInitialize();

		virtual void beforeDraw(float dt, RHIDrawInfo& drawinfo)override;
		virtual void drawPass(float dt, RHIDrawInfo& drawinfo)override;

		virtual void setup_DescriptorSetLayout()override;
		virtual void setup_RenderPass()override;
		virtual void setup_Pipeline()override;
		virtual void setup_FrameBuffer()override;

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
		void updateUniformUboData(uint32_t windowindex);//在每个具体的相机视角输入
		void updateUboBuffer(RHIDrawInfo& info);
		void updateMVPDescriptorSet();
		void updateImageDescriptorSet();

		void dirtyUniformBuffer();
	private:
		void cacheUniformObject(const TBMath::Mat44& proj_view_matrix);
		void cacheUniformDynamicObject();

		void setupDescriptorSet();

		//Descriptor m_descriptor;
		//std::vector<Entity> m_camera;//camera应该包含pass，删除

		uint32_t m_renderCount = 0;//render entities number

		RHIBufferResource* m_uniformbuffer;//通用uniform
		UniformBufferObject m_uniform_buffer_object;//vp矩阵

		RHIBufferResource* m_uniformdynamicbuffer;//动态uniform
		std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;

	};
}