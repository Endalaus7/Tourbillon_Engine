#pragma once
#include "Component.h"
#include "common.hpp"
#include <set>
#include <queue>
#include <atomic>
#include <typeindex>
#include <typeinfo>
//提供了几种功能：引用计数、延迟删除及追踪、运行时数据和标记。

namespace TourBillon
{
	//数据的接口
	//支持反射(尚未完成)
	//class AssetsData
	//{
	//public:
	//	virtual ~AssetsData() = default;
	//};
	//保存存储路径和AssetsData的指针

	namespace AssetsInfo
	{
		enum DeleteType
		{
			Immedia_Delete,//立即删除,m_delayFrame是转为强制删除的需要帧数
			Immedia_Force_Delete,//强制删除，不考虑当前帧的忙碌情况
			Delay_Render_Delete,//延迟删除,m_delayFrame是渲染帧数
			Delay_Physical_Delete,//延迟删除,m_delayFrame是物理帧数
			No_Delete,//不删除
		};
	}


	template<typename T>
	class Assets : public Component
	{
	public:
		static constexpr size_t _field_count_ = 0;
	public:
		friend class AssetsManager;
		//Assets(const Assets& other);
		virtual ~Assets(){}

		//释放方式
		
		void setAssetPath(const ReflectPath& path){ assetpath = path; }
		const ReflectPath& getAssetPath() { return assetpath; }

		void setAssetData(T* data) { assetdata = data; }
		T* getData() { return assetdata; }

		virtual void insertData() 
		{
			
			AssetsManager::Instance()->registerAsset<T>(this);
		}
		virtual void releaseData()
		{
			AssetsManager::Instance()->releaseAssert<T>(this);//清除资源

			assetpath.clear();
			assetdata = nullptr;
		}
		
		void setDelayFrame(uint32_t frame) { m_delayFrame = frame; }
		void setDeleteType(AssetsInfo::DeleteType type) { deleteType = type; }
		AssetsInfo::DeleteType getDeleteType() { return deleteType; }
	private:
		//加载数据的实现
		virtual T* loadData() = 0;
	protected:
		

		AssetsInfo::DeleteType deleteType = AssetsInfo::Immedia_Delete;
		T* assetdata = nullptr;
		uint32_t m_delayFrame = 0;
		ReflectPath assetpath;//资源的绝对路径，唯一标识
		
	};


	//管理Assets资源
	//不记录数据，记引用计数和引用指针
	class AssetsManager : public Singleton<AssetsManager>
	{
	public:
		void tickRender(float dt);

		//添加资源
		template<typename T>
		void registerAsset(Assets<T>* asset)
		{
			if (!asset || asset->assetpath.empty())
				return;

			std::type_index typeIndex = std::type_index(typeid(T));
			if (m_allassets.find(typeIndex) == m_allassets.end())//未注册该类型
			{
				m_allassets.insert({ typeIndex, AssetsPtr() });
			}
			else//已注册类型
			{
				auto& ptr = m_allassets[typeIndex].data_ref;
				if (ptr.find(asset->assetpath) != ptr.end())//已存在data
				{
					ptr[asset->assetpath]++;
					return;
				}
			}

			//优化：如果要加载的data在待删除列表，直接使用
			for (auto deferdelete_itr = m_DeferredDeleteDatas.begin(); deferdelete_itr != m_DeferredDeleteDatas.end(); deferdelete_itr++)
			{
				if (asset->assetpath == deferdelete_itr->path)
				{
					asset->assetdata = (T*)deferdelete_itr->data;
					m_DeferredDeleteDatas.erase(deferdelete_itr);
					m_allassets[typeIndex].data_ref.insert({ asset->assetpath, 1 });
					return;
				}
			}

			//有资源路径，加载资源
			if (!asset->assetpath.empty())
			{
				bool loadResult = asset->loadData();

				if (!loadResult)
				{
					LOG_ERROR("load asset error");
				}
				m_allassets[typeIndex].data_ref.insert({ asset->assetpath, 1 });
				return;
			}
		}
		template<typename T>
		bool releaseAssert(Assets<T>* asset)
		{
			if (!asset)
				return false;

			std::type_index typeIndex = std::type_index(typeid(T));
			if (m_allassets.find(typeIndex) == m_allassets.end())//未注册该类型
				return false;

			auto& ptr = m_allassets[typeIndex].data_ref;
			if (ptr.find(asset->assetpath) == ptr.end())
				return false;

			deleteAssert(asset);
		}
		//删除资源
		template<typename T>
		bool deleteAssert(Assets<T>* asset)
		{
			if (asset->deleteType == AssetsInfo::Immedia_Force_Delete)
			{
				deleteData((void*)asset->assetdata);
			}
			else if (asset->deleteType == AssetsInfo::Immedia_Delete)
			{
				if (m_deleteInCurrFrame > m_max_deleted_inFrame)
				{
					m_PendingDeletes.push(asset->assetdata);
				}
				else
				{
					deleteData((void*)asset->assetdata);
				}
			}
			else if (asset->deleteType == AssetsInfo::Delay_Render_Delete)
			{
				AssetToDelete deferred_delete;
				deferred_delete.data = asset->assetdata;
				deferred_delete.path = asset->assetpath;
				deferred_delete.FrameDeleted = asset->m_delayFrame;
				m_DeferredDeleteDatas.push_back(deferred_delete);
			}
			else if(asset->deleteType == AssetsInfo::No_Delete)
			{
				return false;
			}
			else//未实现或未知删除方式
			{
				//delete data;
				return false;
			}
			return true;
		}
		void deleteData(void* assetdata);

		//用于记录所有指向该data的指针

		struct AssetsPtr {
			std::unordered_map<std::string ,uint32_t> data_ref;
			//std::atomic<uint32_t>* ref_count = 0;//引用计数
		};

		std::unordered_map<std::type_index, AssetsPtr> m_allassets;//所有assetData指针
		//std::unordered_map<uint32_t, uint32_t> m_refs;

		// 待删除的资源.
		std::queue<void*> m_PendingDeletes;
		// 正在删除的资源.
		void* CurrentlyDeleting;

		uint32_t m_max_deleted_inFrame = 100;//一帧中最大释放资源数
		uint32_t m_deleteInCurrFrame = 0;//当前帧释放资源数
		struct AssetToDelete
		{
			ReflectPath path;		//标记资源的绝对路径
			void* data;    // 待删除的资源.
			uint32_t    FrameDeleted; // 等待的帧数.
		};
		// 延迟删除的资源队列.
		std::vector<AssetToDelete> m_DeferredDeleteDatas;
		uint32_t m_CurrentFrame;//判断延迟删除
	};
}

