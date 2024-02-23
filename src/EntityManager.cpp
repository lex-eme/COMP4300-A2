#include "EntityManager.h"

EntityManager::EntityManager()
{
	
}

void EntityManager::update()
{
	for (auto e : m_EntitiesToAdd)
	{
		m_Entities.push_back(e);
		m_EntityMap[e->m_Tag].push_back(e);
	}

	m_EntitiesToAdd.clear();
	removeDeadEntities(m_Entities);

	for (auto& [tag, entityVec] : m_EntityMap)
	{
		removeDeadEntities(entityVec);
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(++m_TotalEntities, tag));
	m_EntitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_Entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_EntityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_EntityMap;
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	auto it = vec.begin();
	while (it != vec.end())
	{
		if (!(it->get()->isActive()))
		{
			it = vec.erase(it);
		}

		if (it != vec.end())
			it++;
	}
}