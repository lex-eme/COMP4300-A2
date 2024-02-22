#pragma once

#include "Entity.h"
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
	EntityVec	m_Entities;
	EntityVec	m_EntitiesToAdd;
	EntityMap	m_EntityMap;
	size_t		m_TotalEntities = 0;

public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
	const std::map<std::string, EntityVec>& getEntityMap();

private:
	void removeDeadEntities(EntityVec& vec);
};

