#include "Entity.hpp"
#include <cassert>

Entity::Entity(std::string tag, size_t id)
	: m_tag(std::move(tag)), m_id(id)
{
}

void Entity::destroy()
{
	m_alive = false;
}

bool Entity::isAlive() const
{
	return m_alive;
}

const std::string& Entity::tag() const
{
	return m_tag;
}

size_t Entity::id() const
{
	return m_id;
}
