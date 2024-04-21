#pragma once
#include "Entity.hpp"
#include <vector>
#include <map>
#include <memory>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
public:
	void init(std::vector<std::string> toAddTags);
	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);

private:
	size_t m_entitiesCount = 0;
	EntityVec m_entities;
	std::map<std::string, EntityVec> m_entityMap;
	EntityVec m_toAdd;
};

