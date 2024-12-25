#include "Assets.h"

TourBillon::Assets::~Assets()
{
	release();
}

FORCE_INLINE const ReflectPath& TourBillon::Assets::getAssetPath()
{
	return assetpath;
}

FORCE_INLINE void TourBillon::Assets::setAssetPath(const ReflectPath& path)
{
	assetpath = path;
	//release asset
	release();
	if(AssetsManager::Instance()->registerAsset(path))
	{
		//reload asset
		loadData();
	}
}

TourBillon::AssetsData* TourBillon::Assets::getData()
{
	return assetdata;
}

void TourBillon::Assets::release()
{
	if(AssetsManager::Instance()->releaseAsset(assetpath))
	{
		
	}
	assetpath.clear();
	assetdata = nullptr;
}

void TourBillon::AssetsManager::tickRender(float dt)
{
	
}

bool TourBillon::AssetsManager::registerAsset(const ReflectPath& path)
{
	auto itr_ref = m_refs.find(path);
	if (itr_ref != m_refs.end())
	{
		itr_ref->second++;
		return false;
	}
	return true;
}

bool TourBillon::AssetsManager::releaseAsset(const ReflectPath& path)
{
	auto itr_ref = m_refs.find(path);
	if (itr_ref != m_refs.end() && --itr_ref->second == 0)
	{
		m_refs.erase(itr_ref);
		return true;
	}
	return false;
}
