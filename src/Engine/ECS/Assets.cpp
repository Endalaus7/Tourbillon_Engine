#include "Assets.h"




void TourBillon::AssetsManager::tickRender(float dt)
{
	m_deleteInCurrFrame = 0;

	while (!m_PendingDeletes.empty())
	{
		if (m_deleteInCurrFrame > m_max_deleted_inFrame)
			break;
		deleteData(m_PendingDeletes.front());
		m_PendingDeletes.pop();
	}
	uint32_t deferred_delete_count = 0;//��֡�ӳ�ɾ��data����
	for (auto itr = m_DeferredDeleteDatas.begin(); itr!= m_DeferredDeleteDatas.end();itr++)
	{
		if (--itr->FrameDeleted == 0)
		{
			if (m_deleteInCurrFrame > m_max_deleted_inFrame)
				m_PendingDeletes.push(itr->data);
			else
			{
				deleteData(itr->data);
				m_deleteInCurrFrame++;
			}
			itr->path.clear();
			itr->data = nullptr;
		}
		else//��ɾ����ǰ��
		{
			auto move_itr = itr;
			std::advance(move_itr, -deferred_delete_count);
			move_itr = itr;
		}
	}
	m_DeferredDeleteDatas.resize(m_DeferredDeleteDatas.size() - deferred_delete_count);
}

void TourBillon::AssetsManager::deleteData(void* assetdata)
{
	if (m_deleteInCurrFrame > m_max_deleted_inFrame)
		CurrentlyDeleting = assetdata;
	delete assetdata;
	m_deleteInCurrFrame++;
}

