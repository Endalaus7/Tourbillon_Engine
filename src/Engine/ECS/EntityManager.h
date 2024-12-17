#pragma once
#include "ECSCommon.h"
#include <queue>
#include <array>



namespace TourBillon
{
	class EntityManager
	{
	public:
		EntityManager();

		void Init();
		Entity CreateEntity();
		void DestroyEntity(Entity entity);
		void SetSignature(Entity entity, Signature signature);

		Signature GetSignature(Entity entity);
	protected:
		std::queue<Entity> m_entityPool{};
		std::array<Signature, MAX_ENTITIES> mSignatures;
		Entity m_currEntitiesNum = 0;
	};
}

