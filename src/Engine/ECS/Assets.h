#pragma once
#include "Component.h"
#include "common.hpp"


//�ṩ�˼��ֹ��ܣ����ü������ӳ�ɾ����׷�١�����ʱ���ݺͱ�ǡ�

namespace TourBillon
{
	//���ݵĽӿ�
	//֧�ַ���(��δ���)
	class AssetsData
	{
		virtual ~AssetsData() = default;
	};
	//����洢·����AssetsData��ָ��
	class Assets : public Component
	{
	public:
		//�ͷŷ�ʽ
		enum DeleteType
		{
			Immedia_Release,//����ɾ��
			Delay_Release,//�ӳ�ɾ��
		};
		~Assets();
		FORCE_INLINE const ReflectPath& getAssetPath();
		void setAssetPath(const ReflectPath& path);

		
		
		FORCE_INLINE AssetsData* getData();
		void release();//���ͷ�ָ��

		DeleteType releaseType;
	protected:
		virtual void loadData() = 0;

		AssetsData* assetdata;
		uint32_t m_deleyFrame = 0;
		ReflectPath assetpath;//����·��,���ڱ�ʶΨһ�ʲ�
		
	};


	//����Assets��Դ����������ֽӿ�
	//����¼���ݣ�ֻ�����ü���
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

