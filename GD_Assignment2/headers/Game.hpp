#pragma once
#include "Entity.hpp"
#include "Config.hpp"
#include <SFML/Graphics.hpp>
#include <EntityManager.hpp>

class Game
{
public:
	Game(const std::string title = "COMP4300 Assignment 2!", const std::string configFilePath="res/config.txt");
	~Game();

	void init();
	void update();
	void run();

	void DrawImGuiUI(sf::Time deltaTime);

	// Systems
	void sUserInput();
	void sMovement();
	void sCollision();
	void sScore();
	void sRender();
	void sEnemySpawner();
	void sPlayerWeapon();
	void sLifetimeManagement();

	void shootEnemy(std::shared_ptr<Entity>& e);

	[[nodiscard]] inline bool isRunning() const noexcept
	{
		return m_running;
	}

private:
	const Config config;
	sf::RenderWindow m_window;
	sf::Clock m_deltaClock;
	EntityManager m_entityManager;
	std::shared_ptr<Entity> m_player;
	bool m_running = true;
	bool m_paused = false;

	sf::Font m_font;
	sf::Texture m_backTex;
	sf::Sprite m_background;
};
