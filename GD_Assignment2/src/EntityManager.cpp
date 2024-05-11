#include "EntityManager.hpp"
#include <cassert>
#include <iostream>

void EntityManager::init(const std::vector<std::string> toAddTags)
{
	for (const std::string& tag : toAddTags) {
		auto e = std::shared_ptr<Entity>(new Entity(tag, m_entitiesCount++));
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

	// remove dead entities
	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](const auto& e) {return !e->isAlive(); }), m_entities.end());

	for (auto& pair : m_entityMap)
	{
		pair.second.erase(std::remove_if(pair.second.begin(), pair.second.end(), [](const auto& e) {return !e->isAlive(); }), pair.second.end());
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	std::shared_ptr<Entity> entity = std::shared_ptr<Entity>(new Entity(tag, m_entitiesCount++));
	m_toAdd.push_back(entity);
	return entity;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

