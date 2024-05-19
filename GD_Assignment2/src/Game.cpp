#include "Game.hpp"
#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>


Game::Game(const std::string title, const std::string configFilePath)
	: config(configFilePath), m_window(sf::VideoMode(config.windowWidth, config.windowHeight), title)
{
	m_window.setFramerateLimit(config.windowFPS);

	// initialize IMGUI and create a clock used for its internal timing
	ImGui::SFML::Init(m_window);

	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;
	ImGui::CaptureMouseFromApp(true);

	// scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	m_deltaClock.restart();
}

Game::~Game()
{
	ImGui::SFML::Shutdown(m_window);
}

void Game::init()
{
	srand((unsigned int)time(0));

	m_scenes["main"] = std::make_shared<ScenePlay>("res/config.txt", this);
	currentScene = m_scenes["main"];
}

void Game::update()
{
	sUserInput();
	sRender();
}

void Game::run()
{
	while (isRunning()) {
		if (!currentScene->hasEnded && !currentScene->paused) {
			currentScene->update(m_deltaClock.restart());
		}
		update();
	}
}

void Game::sUserInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(m_window, event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			m_running = false;
			break;
		case sf::Event::Resized:
		{
			// TODO(CP) : Do it properly 
			const sf::Vector2u ws(event.size.width, event.size.height);
			sf::View view = m_window.getView();
			view.setSize(ws.x, ws.y);
			view.setCenter(ws.x / 2.0f, ws.y / 2.0f);
			m_window.setView(view);

			std::cout << "Window Resized " << ws.x << ", " << ws.y << 
			"\nNew Centre: "<<ws.x/2 << ", "<<ws.y/2 << std::endl;
		}
			break;
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				currentScene->paused = !currentScene->paused;
				m_paused = !m_paused;
				break;
			case sf::Keyboard::W:
				cInput->wPressed = true;
				break;
			case sf::Keyboard::A:
				cInput->aPressed = true;
				break;
			case sf::Keyboard::S:
				cInput->sPressed = true;
				break;
			case sf::Keyboard::D:
				cInput->dPressed = true;
				break;
			}
		}
		break;
		case sf::Event::KeyReleased:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				cInput->wPressed = false;
				break;
			case sf::Keyboard::A:
				cInput->aPressed = false;
				break;
			case sf::Keyboard::S:
				cInput->sPressed = false;
				break;
			case sf::Keyboard::D:
				cInput->dPressed = false;
				break;
			}
		}
		break;
		case sf::Event::MouseButtonPressed:
		{
			if (ImGui::GetIO().WantCaptureMouse) continue;
			switch (event.key.code)
			{
			case sf::Mouse::Left:
				cInput->leftMouse = true;
				break;
			case sf::Mouse::Right:
				cInput->rightMouse = true;
				break;
			}
		}
		break;
		case sf::Event::MouseButtonReleased:
		{
			if (ImGui::GetIO().WantCaptureMouse) continue;
			switch (event.key.code)
			{
			case sf::Mouse::Left:
				cInput->leftMouse = false;
				break;
			case sf::Mouse::Right:
				cInput->rightMouse = false;
				break;
			}
		}
		break;
		default:
			break;
		}
	}
}

void Game::sRender()
{
	m_window.display();
}

