#include "Entity.h"

Entity::Entity(const size_t id, const std::string& tag)
	: m_ID(id), m_Tag(tag)
{
}

bool Entity::isActive() const
{
	return m_Active;
}

const std::string& Entity::tag() const
{
	return m_Tag;
}

const size_t Entity::id() const
{
	return m_ID;
}

void Entity::destroy()
{
	m_Active = false;
}
