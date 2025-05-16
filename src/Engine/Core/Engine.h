
#pragma once
#include <memory>


namespace TourBillon
{
	struct EngineInitInfo{
		//uint32_t window_width = 500;
		//uint32_t window_height = 500;
		//uint32_t window_num = 1;
	};
	class RenderSystem;
	class TBEngine
	{
	public:
		virtual void initialize(EngineInitInfo engine_init_info);
		virtual void run();

		void UpdateBeforeRender(float dt);
		void UpdateAfterRender(float dt);
	protected:
		std::shared_ptr<RenderSystem> m_renderSystem;
	};
}