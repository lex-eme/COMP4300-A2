#include "EntityManager.h"

void EntityManager::removeDeadEntities(EntityVec& vec)
{
}

EntityManager::EntityManager()
{
	// TODO: remove all dead entities from the input vector
	// this is called by the update() function
}

void EntityManager::update()
{
	// TODO: add entites from m_EntitiesToAdd to the proper location(s)
	//		- add them to the vector of all entities
	//		- add them to the vector inside the map, with the tag as a key

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_Entities);

	// remove dead entities from each vector in the map
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
	// TODO: this is incorrect, return the correct vector from the map
	return m_Entities;
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_EntityMap;
}
