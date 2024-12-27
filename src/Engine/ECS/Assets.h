#pragma once
#include "Component.h"
#include "common.hpp"
#include <set>
#include <queue>

//提供了几种功能：引用计数、延迟删除及追踪、运行时数据和标记。

namespace TourBillon
{
	//数据的接口
	//支持反射(尚未完成)
	class AssetsData
	{
	public:
		virtual ~AssetsData() = default;
	};
	//保存存储路径和AssetsData的指针

	class Assets : public Component
	{
	public:
		friend class AssetsManager;

		Assets();
		Assets(const Assets& other);
		virtual ~Assets();

		Assets& operator=(const Assets& other);

		//释放方式
		enum DeleteType
		{
			Immedia_Delete,//立即删除,m_delayFrame是转为强制删除的需要帧数
			Immedia_Force_Delete,//强制删除，不考虑当前帧的忙碌情况
			Delay_Render_Delete,//延迟删除,m_delayFrame是渲染帧数
			Delay_Physical_Delete,//延迟删除,m_delayFrame是物理帧数
		};
		const ReflectPath& getAssetPath();
		void setAssetPath(const ReflectPath& path);

		//仅IO系统可调用
		void setAssetData(AssetsData* data) { assetdata = data; }
		//直接设置data和名称
		void setAssetData(const ReflectPath& name,AssetsData* data);
		
		AssetsData* getData();
		void release();//仅释放指针

		virtual void insertData()override;
		virtual void releaseData()override;
		
		void setDelayFrame(uint32_t frame) { m_delayFrame = frame; }
		void setDeleteType(DeleteType type) { deleteType = type; }
		DeleteType getDeleteType() { return deleteType; }
	private:
		//加载数据的实现
		virtual AssetsData* loadData() = 0;
	protected:
		uint32_t ref_index = 0;//用引用计数给指针标号

		DeleteType deleteType = Immedia_Delete;
		AssetsData* assetdata = nullptr;
		uint32_t m_delayFrame = 0;
		ReflectPath assetpath;//绝对路径,用于标识唯一资产
		

		
	};


	//管理Assets资源
	//不记录数据，记引用计数和引用指针
	class AssetsManager : public Singleton<AssetsManager>
	{
		friend class Assets;
	private:
		
	public:
		void tickRender(float dt);

	private:
		uint32_t getRef(const ReflectPath& path);
		//
		bool registerAsset(Assets* asset);
		//if m_ref == 0,return true(true means delete data)
		bool releaseAsset(Assets* asset);
		//force delete all asset
		void forceReleaseAsset(const ReflectPath& path);

		bool deleteAssert(Assets* asset);
		void deleteData(AssetsData*& assetdata);

		std::set<Assets*> m_allassets;//所有asset指针
		std::unordered_map<ReflectPath, uint32_t> m_refs;

		// 待删除的资源.
		std::queue<AssetsData*> m_PendingDeletes;
		// 正在删除的资源.
		AssetsData* CurrentlyDeleting;

		uint32_t m_max_deleted_inFrame = 100;//一帧中最大释放资源数
		uint32_t m_deleteInCurrFrame = 0;//当前帧释放资源数
		struct AssetToDelete
		{
			ReflectPath path;		//标记资源的绝对路径
			AssetsData* data;    // 待删除的资源.
			uint32_t    FrameDeleted; // 等待的帧数.
		};
		// 延迟删除的资源队列.
		std::vector<AssetToDelete> m_DeferredDeleteDatas;
		uint32_t m_CurrentFrame;//判断延迟删除
	};
}

