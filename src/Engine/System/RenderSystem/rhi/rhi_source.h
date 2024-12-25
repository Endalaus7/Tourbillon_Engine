#pragma once

#include "rhi_type.h"
#include "Structure/TB_AlignedArray.hpp"
#include <memory>
#include <queue>
#include "rhi_struct.h"

//��ʱû���ã��ڴ���Դ�������ƶ���ecs�У��ṩ������systemʹ�á�
//��ʱRHIResource������Ϊ�ڴ���Դ������rhi���ʵ��

//RHIResource�ǳ�����gpu�����Դ


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
		void Release_flush();//ˢ��
		void Release_now(RHIResource*& resource);

		bool g_RHINeedsExtraDeletionLatency = true;//���� GPU ��Դ���ӳ�ɾ��
		const size_t m_max_deleted_inFrame = 100;//һ֡������ͷ���Դ��
	
		uint32_t m_deleteInCurrFrame = 0;//��ǰ֡ɾ����Դ��
		// ��ɾ������Դ.
		std::queue<RHIResource*> PendingDeletes;
		// ����ɾ������Դ.
		RHIResource* CurrentlyDeleting;

		struct ResourcesToDelete
		{
			RHIResource*    Resource;    // ��ɾ������Դ.
			uint32_t                    FrameDeleted; // �ȴ���֡��.
		};
		// �ӳ�ɾ������Դ����.
		std::vector<ResourcesToDelete> DeferredDeletionQueue;
		uint32_t CurrentFrame;//�ж��ӳ�ɾ��
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