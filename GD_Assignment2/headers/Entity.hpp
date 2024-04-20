#pragma once
#include <string>
#include "Components.hpp"

class Entity
{
public: // TODO(CP) : make it private, only accessible via EntityManager
	Entity();
	Entity(std::string m_tag, size_t id);
public:
	~Entity();

	void destroy();
	bool isActive();
	std::string& tag();
	size_t id();

private:
	std::string m_tag;
	bool m_active;
	size_t m_id;

	CTransform* cTransform = nullptr;
	CShape* cShape = nullptr;
	CCollision* cCollision = nullptr;
	CInput* cInput = nullptr;
	CScore* cScore = nullptr;
	CLifespan* cLifespan = nullptr;
};
