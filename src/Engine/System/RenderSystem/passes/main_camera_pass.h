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

		virtual void drawPass(float dt, RHIDrawInfo& drawinfo)override;

		virtual void setup_DescriptorSetLayout()override;
		virtual void setup_RenderPass()override;
		virtual void setup_Pipeline()override;
		virtual void setup_FrameBuffer()override;

		

		void setMainCamera(Entity camera);

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
		void updateDescriptorSet();

		void dirtyUniformBuffer();
	private:
		void cacheUniformObject(const TBMath::Mat44& proj_view_matrix);
		void cacheUniformDynamicObject();

		void setupDescriptorSet();

		//Descriptor m_descriptor;
		Entity m_camera;

		uint32_t m_renderCount = 0;//render entities number

		RHIBufferResource* m_uniformbuffer;//Õ®”√uniform
		UniformBufferObject m_uniform_buffer_object;//vpæÿ’Û

		RHIBufferResource* m_uniformdynamicbuffer;//∂ØÃ¨uniform
		std::vector<UniformBufferDynamicObject> m_uniform_buffer_dynamic_object_cache;
	};
}