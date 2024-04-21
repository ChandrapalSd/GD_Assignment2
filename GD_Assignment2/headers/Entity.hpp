#pragma once
#include <string>
#include "Components.hpp"

class Entity
{
public: // TODO(CP) : make it private, only accessible via EntityManager
	Entity();
	Entity(std::string m_tag, size_t id);
public:

	void destroy();
	bool isAlive();
	std::string& tag();
	size_t id();

private:
	std::string m_tag = "notag";
	bool m_alive = true;
	size_t m_id = 0;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;

	friend class EntityManager;
};
