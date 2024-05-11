#pragma once
#include <string>
#include "Components.hpp"

class Entity
{
private:
	Entity() = delete;
	Entity(std::string m_tag, size_t id);
public:

	void destroy();
	bool isAlive() const;
	const std::string& tag() const;
	size_t id() const;

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
	std::shared_ptr<CGun> cGun;

	friend class EntityManager;
};
