#pragma once

#include "rhi_type.h"
#include "Structure/TB_AlignedArray.hpp"
#include <memory>
#include <queue>
#include "rhi_struct.h"

//暂时没有用，内存资源管理即将移动到ecs中，提供给更多system使用。
//届时RHIResource可以作为内存资源管理在rhi层的实现

//RHIResource是抽象了gpu侧的资源


namespace TourBillon
{

	class RHIResource 
	{
	public:
		RHIResource();
		virtual ~RHIResource();

		//virtual void init(RHISourceInitInfo rhi_source_init_info) = 0;

		uint32_t DeferDeleteFrame = 0;
	protected:
		bool bCommitted;

	};

	class RHIResourceManager :public Singleton<RHIResourceManager>
	{
	public:
		void tick(float dt);
		void Release(RHIResource* resource, uint32_t waitframe = 0);
	private:
		void Release_flush();//刷新
		void Release_now(RHIResource*& resource);

		bool g_RHINeedsExtraDeletionLatency = true;//控制 GPU 资源的延迟删除
		const size_t m_max_deleted_inFrame = 100;//一帧中最大释放资源数
	
		uint32_t m_deleteInCurrFrame = 0;//当前帧删除资源数
		// 待删除的资源.
		std::queue<RHIResource*> PendingDeletes;
		// 正在删除的资源.
		RHIResource* CurrentlyDeleting;

		struct ResourcesToDelete
		{
			RHIResource*    Resource;    // 待删除的资源.
			uint32_t                    FrameDeleted; // 等待的帧数.
		};
		// 延迟删除的资源队列.
		std::vector<ResourcesToDelete> DeferredDeletionQueue;
		uint32_t CurrentFrame;//判断延迟删除
	};


/*####################################    Resources   ############################################*/

	//
	class RHITextureResource :public RHIResource
	{
	public:
		RHIColorValue ClearValue;
		uint32_t NumMips;
		uint32_t NumSamples;
		RHIFormat Format;
		RHICommonFlags Flags;
		RHIName TextureName;
	};

	class RHIBufferResource :public RHIResource
	{
	public:
		RHIBuffer* buffer = nullptr;
		RHIDeviceMemory* buffermemory = nullptr;
	};
}