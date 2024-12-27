#pragma once
#include "Component.h"
#include "common.hpp"
#include <set>
#include <queue>

//�ṩ�˼��ֹ��ܣ����ü������ӳ�ɾ����׷�١�����ʱ���ݺͱ�ǡ�

namespace TourBillon
{
	//���ݵĽӿ�
	//֧�ַ���(��δ���)
	class AssetsData
	{
	public:
		virtual ~AssetsData() = default;
	};
	//����洢·����AssetsData��ָ��

	class Assets : public Component
	{
	public:
		friend class AssetsManager;

		Assets();
		Assets(const Assets& other);
		virtual ~Assets();

		Assets& operator=(const Assets& other);

		//�ͷŷ�ʽ
		enum DeleteType
		{
			Immedia_Delete,//����ɾ��,m_delayFrame��תΪǿ��ɾ������Ҫ֡��
			Immedia_Force_Delete,//ǿ��ɾ���������ǵ�ǰ֡��æµ���
			Delay_Render_Delete,//�ӳ�ɾ��,m_delayFrame����Ⱦ֡��
			Delay_Physical_Delete,//�ӳ�ɾ��,m_delayFrame������֡��
		};
		const ReflectPath& getAssetPath();
		void setAssetPath(const ReflectPath& path);

		//��IOϵͳ�ɵ���
		void setAssetData(AssetsData* data) { assetdata = data; }
		//ֱ������data������
		void setAssetData(const ReflectPath& name,AssetsData* data);
		
		AssetsData* getData();
		void release();//���ͷ�ָ��

		virtual void insertData()override;
		virtual void releaseData()override;
		
		void setDelayFrame(uint32_t frame) { m_delayFrame = frame; }
		void setDeleteType(DeleteType type) { deleteType = type; }
		DeleteType getDeleteType() { return deleteType; }
	private:
		//�������ݵ�ʵ��
		virtual AssetsData* loadData() = 0;
	protected:
		uint32_t ref_index = 0;//�����ü�����ָ����

		DeleteType deleteType = Immedia_Delete;
		AssetsData* assetdata = nullptr;
		uint32_t m_delayFrame = 0;
		ReflectPath assetpath;//����·��,���ڱ�ʶΨһ�ʲ�
		

		
	};


	//����Assets��Դ
	//����¼���ݣ������ü���������ָ��
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

		std::set<Assets*> m_allassets;//����assetָ��
		std::unordered_map<ReflectPath, uint32_t> m_refs;

		// ��ɾ������Դ.
		std::queue<AssetsData*> m_PendingDeletes;
		// ����ɾ������Դ.
		AssetsData* CurrentlyDeleting;

		uint32_t m_max_deleted_inFrame = 100;//һ֡������ͷ���Դ��
		uint32_t m_deleteInCurrFrame = 0;//��ǰ֡�ͷ���Դ��
		struct AssetToDelete
		{
			ReflectPath path;		//�����Դ�ľ���·��
			AssetsData* data;    // ��ɾ������Դ.
			uint32_t    FrameDeleted; // �ȴ���֡��.
		};
		// �ӳ�ɾ������Դ����.
		std::vector<AssetToDelete> m_DeferredDeleteDatas;
		uint32_t m_CurrentFrame;//�ж��ӳ�ɾ��
	};
}

