#pragma once
#include "Entity.hpp"
#include <vector>
#include <map>
#include <memory>

class EntityManager
{
public:
	void init();
	void update();
	std::shared_ptr<Entity> addEntity();
	std::vector<std::shared_ptr<Entity>>& getEntities();
	//std::vector<std::shared_ptr<Entity>>& getEntities(s);

private:
	std::vector<Entity> m_entities;
	std::map<std::string, Entity> m_entityMap;
	std::vector<Entity> m_toAdd;
};

