#include "Entity.hpp"
#include <cassert>

Entity::Entity()
{
	// TODO(CP) : Not implemented
}

Entity::Entity(std::string tag, size_t id)
	: m_tag(std::move(tag)), m_id(id)
{
}

Entity::~Entity()
{
	// TODO(CP) : Not implemented
}

void Entity::destroy()
{
	// TODO(CP) : Not implemented
	assert(false);
}

bool Entity::isActive()
{
	// TODO(CP) : Not implemented
	assert(false);
	return false;
}

std::string& Entity::tag()
{
	return m_tag;
}

size_t Entity::id()
{
	return m_id;
}
