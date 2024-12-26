#include "Assets.h"


TourBillon::Assets::Assets()
{
	setAssetData("",nullptr);
}

TourBillon::Assets::Assets(const Assets& other)
{
	setAssetData(other.assetpath, other.assetdata);
}

TourBillon::Assets::~Assets()
{
	release();
}


TourBillon::Assets& TourBillon::Assets::operator=(const Assets& other)
{
	setAssetData(other.assetpath, other.assetdata);
	return *this;
}

const ReflectPath& TourBillon::Assets::getAssetPath()
{
	return assetpath;
}

void TourBillon::Assets::setAssetPath(const ReflectPath& path)
{
	release();
	assetpath = path;
	AssetsManager::Instance()->registerAsset(this);
	
}

void TourBillon::Assets::setAssetData(const ReflectPath& name, AssetsData* data)
{
	//if (AssetsManager::Instance()->getRef(name))//已有同名注册
	//{
	//	LOG_WINDOWBOX("Name ["+ name +"] has been registered");
	//	return;
	//}
	release();
	assetpath = name;
	assetdata = data;
	AssetsManager::Instance()->registerAsset(this, true);
}

TourBillon::AssetsData* TourBillon::Assets::getData()
{
	return assetdata;
}

void TourBillon::Assets::release()
{
	if (!assetdata)
		return;
	if(!AssetsManager::Instance()->releaseAsset(this))//清除资源
	{
		LOG_ERROR("release error")
	}
	assetpath.clear();
	assetdata = nullptr;
}




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
	uint32_t deferred_delete_count = 0;//本帧延迟删除data数量
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
		else//非删除项前移
		{
			auto move_itr = itr;
			std::advance(move_itr, -deferred_delete_count);
			move_itr = itr;
		}
	}
	m_DeferredDeleteDatas.resize(m_DeferredDeleteDatas.size() - deferred_delete_count);
}

uint32_t TourBillon::AssetsManager::getRef(const ReflectPath& path)
{
	if (m_refs.find(path) == m_refs.end())
		return 0;
	return m_refs[path];
}

bool TourBillon::AssetsManager::registerAsset(Assets* asset, bool usedata)
{
	if(!asset)
		return false;
	
	m_allassets.insert(asset);
	auto itr_ref = m_refs.find(asset->getAssetPath());
	if (itr_ref != m_refs.end())//已有data，直接引用++
	{
		itr_ref->second++;
		return true;
	}
	
	if (usedata)
	{
		m_refs.insert({ asset->getAssetPath(),1 });
		return true;
	}

	//优化：如果注册data在待删除列表，直接使用
	for (auto deferdelete_itr = m_DeferredDeleteDatas.begin(); deferdelete_itr != m_DeferredDeleteDatas.end(); deferdelete_itr++)
	{
		if (deferdelete_itr->path == asset->assetpath)
		{
			asset->assetdata = deferdelete_itr->data;
			m_DeferredDeleteDatas.erase(deferdelete_itr);
			return true;
		}
	}


	//没有data，加载资源
	
	bool loadResult = asset->loadData();

	if (!loadResult)
	{
		LOG_ERROR("load asset error");
		return false;
	}
	m_refs.insert({ asset->getAssetPath(),1 });
	return true;
}

bool TourBillon::AssetsManager::releaseAsset(Assets* asset)
{
	if (!asset)return false;
	m_allassets.erase(asset);
	auto itr_ref = m_refs.find(asset->getAssetPath());
	if (itr_ref != m_refs.end() && --itr_ref->second == 0)//释放引用
	{
		m_refs.erase(itr_ref);

		bool deleteResult = deleteAssert(asset);//删除data
		return deleteResult;
	}
	return true;
}

bool TourBillon::AssetsManager::deleteAssert(Assets* asset)
{
	if (asset->deleteType == Assets::Immedia_Force_Delete)
	{
		deleteData(asset->assetdata);
	}
	else if (asset->deleteType == Assets::Immedia_Delete)
	{
		if (m_deleteInCurrFrame > m_max_deleted_inFrame)
		{
			m_PendingDeletes.push(asset->assetdata);
		}
		else
		{
			deleteData(asset->assetdata);
		}
	}
	else if (asset->deleteType == Assets::Delay_Render_Delete)
	{
		AssetToDelete deferred_delete;
		deferred_delete.data = asset->assetdata;
		deferred_delete.path = asset->assetpath;
		deferred_delete.FrameDeleted = asset->m_delayFrame;
		m_DeferredDeleteDatas.push_back(deferred_delete);
	}
	else//未实现或未知删除方式
	{
		//delete data;
		return false;
	}
}

void TourBillon::AssetsManager::deleteData(AssetsData*& assetdata)
{
	if(m_deleteInCurrFrame > m_max_deleted_inFrame)
	CurrentlyDeleting = assetdata;
	delete assetdata;
	m_deleteInCurrFrame++;
	assetdata = nullptr;
}

void TourBillon::AssetsManager::forceReleaseAsset(const ReflectPath& path)
{
	//因为不常用到，暂时使用遍历方法
	for (auto releaseitr : m_allassets)
	{

	}
}

