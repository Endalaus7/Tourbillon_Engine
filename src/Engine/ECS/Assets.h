#pragma once
#include "Component.h"
#include "common.hpp"


//提供了几种功能：引用计数、延迟删除及追踪、运行时数据和标记。

namespace TourBillon
{
	//数据的接口
	//支持反射(尚未完成)
	class AssetsData
	{
		virtual ~AssetsData() = default;
	};
	//保存存储路径和AssetsData的指针
	class Assets : public Component
	{
	public:
		//释放方式
		enum DeleteType
		{
			Immedia_Release,//立即删除
			Delay_Release,//延迟删除
		};
		~Assets();
		FORCE_INLINE const ReflectPath& getAssetPath();
		void setAssetPath(const ReflectPath& path);

		
		
		FORCE_INLINE AssetsData* getData();
		void release();//仅释放指针

		DeleteType releaseType;
	protected:
		virtual void loadData() = 0;

		AssetsData* assetdata;
		uint32_t m_deleyFrame = 0;
		ReflectPath assetpath;//绝对路径,用于标识唯一资产
		
	};


	//管理Assets资源，不对外表现接口
	//不记录数据，只记引用计数
	class AssetsManager : public Singleton<AssetsManager>
	{
		friend Assets;
	public:
		void tickRender(float dt);

	private:
		bool registerAsset(const ReflectPath& path);//if asset has registered, m_refs++, return false
		bool releaseAsset(const ReflectPath& path);//if m_ref == 0,return true
		std::unordered_map<ReflectPath, uint32_t> m_refs;
	};
}

