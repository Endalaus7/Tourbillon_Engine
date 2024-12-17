#pragma once

#include "rhi_source.h"

namespace TourBillon
{
	TourBillon::RHIResource::RHIResource()
	{

	}

	RHIResource::~RHIResource()
	{
	}
	void RHIResourceManager::tick(float dt)
	{
		m_deleteInCurrFrame = 0;
		Release_flush();
		
	}
	
	void RHIResourceManager::Release(RHIResource* resource, uint32_t waitframe)
	{
		if (waitframe == 0)
		{
			if (g_RHINeedsExtraDeletionLatency)
				Release_now(resource);
			else
				PendingDeletes.push(resource);
		}
		else
		{
			ResourcesToDelete defer_delete_resource;
			defer_delete_resource.Resource = resource;
			defer_delete_resource.FrameDeleted = waitframe;
			DeferredDeletionQueue.push_back(defer_delete_resource);
		}
	}
	void RHIResourceManager::Release_flush()
	{
		decltype(DeferredDeletionQueue) tempDeferredDeletionQueue;
		tempDeferredDeletionQueue.reserve(DeferredDeletionQueue.size());
		for (auto& itr : DeferredDeletionQueue)
		{
			itr.FrameDeleted--;
			if (itr.FrameDeleted > 0)
				tempDeferredDeletionQueue.push_back(itr);
			else
				PendingDeletes.push(itr.Resource);
		}
		DeferredDeletionQueue = tempDeferredDeletionQueue;

		while(!PendingDeletes.empty() && g_RHINeedsExtraDeletionLatency)
		{
			Release_now(PendingDeletes.front());
			PendingDeletes.pop();
		}
	}
	void RHIResourceManager::Release_now(RHIResource*& resource)
	{
		CurrentlyDeleting = resource;
		SAFE_DELETE(resource);
		m_deleteInCurrFrame++;
		if (m_deleteInCurrFrame > m_max_deleted_inFrame)
			g_RHINeedsExtraDeletionLatency = false;
	}
}
