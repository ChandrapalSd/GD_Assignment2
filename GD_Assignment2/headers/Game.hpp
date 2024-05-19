#pragma once
#include "Config.hpp"
#include "scene.hpp"
#include <SFML/Graphics.hpp>

class Game
{
public:
	Game(const std::string title = "COMP4300 Assignment 2!", const std::string configFilePath="res/config.txt");
	~Game();

	void init();
	void update();
	void run();

	// Systems
	void sUserInput();
	void sRender();

	[[nodiscard]] inline bool isRunning() const noexcept
	{
		return m_running;
	}

private:
	const Config config;
	sf::RenderWindow m_window;
	std::map< std::string, std::shared_ptr<Scene>> m_scenes;
	std::shared_ptr<Scene> currentScene = 0;
	sf::Clock m_deltaClock;
	bool m_running = true;
	bool m_paused = false;

	CInput* cInput = new CInput();

	friend class ScenePlay;
};
