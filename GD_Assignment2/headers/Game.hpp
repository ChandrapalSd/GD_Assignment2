#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <EntityManager.hpp>

class Game
{
public:
	Game(uint32_t wWidth, uint32_t wHeight, const std::string title = "COMP4300 Assignment 2!");
	~Game();

	void init(const std::string& filepath);
	void update();
	void run();

	// Systems
	void sUserInput();
	void sMovement();
	void sCollision();
	void sRender();
	void sEnemySpawner();

	[[nodiscard]] inline bool isRunning() const noexcept
	{
		return m_running;
	}

private:
	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	EntityManager m_entityManager;
	std::shared_ptr<Entity> m_player;
	bool m_running = true;
	bool m_paused = false;
};
