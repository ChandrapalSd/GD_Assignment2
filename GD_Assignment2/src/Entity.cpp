#include "Entity.hpp"
#include <cassert>

Entity::Entity(std::string tag, size_t id)
	: m_tag(std::move(tag)), m_id(id)
{
}

void Entity::destroy()
{
	assert(isAlive() && "Entity is already dead!");
	m_alive = false;
}

bool Entity::isAlive()
{
	return m_alive;
}

std::string& Entity::tag()
{
	return m_tag;
}

size_t Entity::id()
{
	return m_id;
}
