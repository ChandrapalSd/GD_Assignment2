#include "EntityManager.hpp"
#include <cassert>
#include <iostream>

void EntityManager::init(const std::vector<std::string> toAddTags)
{
	for (const std::string& tag : toAddTags) {
		auto e = std::make_shared<Entity>(tag, m_entitiesCount++);
		m_entities.push_back(e);
		m_entityMap[tag].push_back(e);
	}
}

void EntityManager::update()
{
	for (auto entity : m_toAdd) {
		m_entities.push_back(entity);
		m_entityMap[entity->tag()].push_back(entity);
	}
	m_toAdd.clear();

	// TODO(CP) : remove dead entities
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(tag, m_entitiesCount++);
	m_toAdd.push_back(entity);
	return entity;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	std::cout << "Entities count ( tag = " << tag << " ) : " << m_entityMap.count(tag);
	assert(m_entityMap.find(tag) != m_entityMap.end() && ("No entities found with tag"));
	return m_entityMap[tag];
}

