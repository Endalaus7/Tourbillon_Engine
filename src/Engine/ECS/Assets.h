#pragma once
#include "Component.h"
#include "common.hpp"
#include <set>
#include <queue>
#include <atomic>
#include <typeindex>
#include <typeinfo>
//�ṩ�˼��ֹ��ܣ����ü������ӳ�ɾ����׷�١�����ʱ���ݺͱ�ǡ�

namespace TourBillon
{
	//���ݵĽӿ�
	//֧�ַ���(��δ���)
	//class AssetsData
	//{
	//public:
	//	virtual ~AssetsData() = default;
	//};
	//����洢·����AssetsData��ָ��

	namespace AssetsInfo
	{
		enum DeleteType
		{
			Immedia_Delete,//����ɾ��,m_delayFrame��תΪǿ��ɾ������Ҫ֡��
			Immedia_Force_Delete,//ǿ��ɾ���������ǵ�ǰ֡��æµ���
			Delay_Render_Delete,//�ӳ�ɾ��,m_delayFrame����Ⱦ֡��
			Delay_Physical_Delete,//�ӳ�ɾ��,m_delayFrame������֡��
			No_Delete,//��ɾ��
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

		//�ͷŷ�ʽ
		
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
			AssetsManager::Instance()->releaseAssert<T>(this);//�����Դ

			assetpath.clear();
			assetdata = nullptr;
		}
		
		void setDelayFrame(uint32_t frame) { m_delayFrame = frame; }
		void setDeleteType(AssetsInfo::DeleteType type) { deleteType = type; }
		AssetsInfo::DeleteType getDeleteType() { return deleteType; }
	private:
		//�������ݵ�ʵ��
		virtual T* loadData() = 0;
	protected:
		

		AssetsInfo::DeleteType deleteType = AssetsInfo::Immedia_Delete;
		T* assetdata = nullptr;
		uint32_t m_delayFrame = 0;
		ReflectPath assetpath;//��Դ�ľ���·����Ψһ��ʶ
		
	};


	//����Assets��Դ
	//����¼���ݣ������ü���������ָ��
	class AssetsManager : public Singleton<AssetsManager>
	{
	public:
		void tickRender(float dt);

		//�����Դ
		template<typename T>
		void registerAsset(Assets<T>* asset)
		{
			if (!asset || asset->assetpath.empty())
				return;

			std::type_index typeIndex = std::type_index(typeid(T));
			if (m_allassets.find(typeIndex) == m_allassets.end())//δע�������
			{
				m_allassets.insert({ typeIndex, AssetsPtr() });
			}
			else//��ע������
			{
				auto& ptr = m_allassets[typeIndex].data_ref;
				if (ptr.find(asset->assetpath) != ptr.end())//�Ѵ���data
				{
					ptr[asset->assetpath]++;
					return;
				}
			}

			//�Ż������Ҫ���ص�data�ڴ�ɾ���б�ֱ��ʹ��
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

			//����Դ·����������Դ
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
			if (m_allassets.find(typeIndex) == m_allassets.end())//δע�������
				return false;

			auto& ptr = m_allassets[typeIndex].data_ref;
			if (ptr.find(asset->assetpath) == ptr.end())
				return false;

			deleteAssert(asset);
		}
		//ɾ����Դ
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
			else//δʵ�ֻ�δ֪ɾ����ʽ
			{
				//delete data;
				return false;
			}
			return true;
		}
		void deleteData(void* assetdata);

		//���ڼ�¼����ָ���data��ָ��

		struct AssetsPtr {
			std::unordered_map<std::string ,uint32_t> data_ref;
			//std::atomic<uint32_t>* ref_count = 0;//���ü���
		};

		std::unordered_map<std::type_index, AssetsPtr> m_allassets;//����assetDataָ��
		//std::unordered_map<uint32_t, uint32_t> m_refs;

		// ��ɾ������Դ.
		std::queue<void*> m_PendingDeletes;
		// ����ɾ������Դ.
		void* CurrentlyDeleting;

		uint32_t m_max_deleted_inFrame = 100;//һ֡������ͷ���Դ��
		uint32_t m_deleteInCurrFrame = 0;//��ǰ֡�ͷ���Դ��
		struct AssetToDelete
		{
			ReflectPath path;		//�����Դ�ľ���·��
			void* data;    // ��ɾ������Դ.
			uint32_t    FrameDeleted; // �ȴ���֡��.
		};
		// �ӳ�ɾ������Դ����.
		std::vector<AssetToDelete> m_DeferredDeleteDatas;
		uint32_t m_CurrentFrame;//�ж��ӳ�ɾ��
	};
}

