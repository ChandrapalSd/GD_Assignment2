#include "Game.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/System/Vector2.hpp>



Game::Game(uint32_t wWidth, uint32_t wHeight)
	: m_window(sf::VideoMode(wWidth, wHeight), "COMP4300 Assignment 2!")
{
	m_window.setFramerateLimit(60);

	// initialize IMGUI and create a clock used for its internal timing
	ImGui::SFML::Init(m_window);

	// scale the imgui ui by a given factor, does not affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);
	m_deltaClock.restart();
}

Game::~Game()
{
	// TODO(CP) : Not implemented
}

void Game::init()
{
	// TODO(CP) : Not implemented
	assert(false);
}

void Game::update()
{

	sf::Event event;

	while (m_window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(m_window, event);

		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::Resized:
			// TODO
			break;
		default:
			break;
		}
	}

	m_window.clear(sf::Color::Black);

	ImGui::SFML::Update(m_window, m_deltaClock.restart());

#ifdef DEMO
	ImGui::ShowDemoWindow();
#endif // DEMO
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Game::sMovement()
{
}

void Game::sUserInput()
{
}

void Game::sRender()
{
}

void Game::sEnemySpawner()
{
}

void Game::sCollision()
{
}
