#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <EntityManager.hpp>

class Game
{
public:
	Game(uint32_t wWidth, uint32_t wHeight);
	~Game();

	void init();
	void update();

	// Systems
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	[[nodiscard]] inline bool isRunning() const noexcept
	{
		return m_window.isOpen();
	}

private:
	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	EntityManager m_entities;
	Entity m_player;
	bool m_running = false;
	// bool m_paused = false;
};
