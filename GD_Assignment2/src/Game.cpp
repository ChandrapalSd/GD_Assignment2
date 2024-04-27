#include "Game.hpp"
#include "Vec2.hpp"
#include "Util.hpp"

#include <iostream>
#include <string>

#include <imgui.h>
#include <imgui-SFML.h>



Game::Game(uint32_t wWidth, uint32_t wHeight, const std::string title)
	: m_window(sf::VideoMode(wWidth, wHeight), title)
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
	ImGui::SFML::Shutdown(m_window);
}


void Game::init(const std::string& filepath)
{
	m_entityManager.init({ "player" });
	m_player = m_entityManager.getEntities("player").front();
	m_player->cTransform = std::make_shared<CTransform>(Vec2(), 5, Vec2());
	m_player->cInput = std::make_shared<CInput>();
	m_player->cShape = std::make_shared<CShape>(10, 6);
	m_player->cCollision = std::make_shared<CCollision>(10);
	m_player->cScore = std::make_shared<CScore>();
	m_player->cShape->shape.setFillColor(sf::Color::Red);

	srand(time(0));
	for (int i = 0; i < 10; i++)
	{
		const float rMin = 10.0, rMax = 50.0;
		float radius = randInRange(rMin, rMax);
		int pCount = randInRange<int>(3, 30);

		std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy");
		e->cTransform = std::make_shared<CTransform>(Vec2(), 5);
		e->cShape = std::make_shared<CShape>(radius, pCount);
		e->cCollision = std::make_shared<CCollision>(radius);
	}
}


void Game::update()
{
	m_entityManager.update();
	m_window.clear(sf::Color::Green);
	ImGui::SFML::Update(m_window, m_deltaClock.restart());

	sUserInput();

	if (!m_paused) {
		sMovement();
		sCollision();
		sRender();
		sEnemySpawner();
	}

#ifdef DEMO
	ImGui::ShowDemoWindow();
#endif // DEMO
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Game::run()
{
	while (isRunning()) {
		update();
	}
}

void Game::sMovement()
{
	m_player->cTransform->velocityNormalized.x = m_player->cInput->dPressed ? 1 : m_player->cInput->aPressed ? -1 : 0;
	m_player->cTransform->velocityNormalized.y = m_player->cInput->sPressed ? 1 : m_player->cInput->wPressed ? -1 : 0;
	m_player->cTransform->velocityNormalized.normalize();

	// Each entity has transform component
	for (auto& e : m_entityManager.getEntities()) {
		double speed = e->cTransform->speed;
		e->cTransform->pos += (e->cTransform->velocityNormalized * speed ); // TODO(CP) : Multiply with deltatime?
		sf::Vector2f newPos((float)e->cTransform->pos.x, (float)e->cTransform->pos.y);
		e->cShape->shape.setPosition(newPos);

		if (e->tag() == "enemy") {

		const auto& windowSize = m_window.getSize();

		const auto& pos = e->cTransform->pos;
		const float& radius = e->cShape->radius;

		// TODO(CP) : Clamp pos to window
		if (pos.x <= 0 || pos.x + radius * 2 >= windowSize.x) {
			e->cTransform->velocityNormalized.x *= -1;
		}
		if (pos.y <= 0 || pos.y + radius * 2 >= windowSize.y) {
			e->cTransform->velocityNormalized.y *= -1;
		}
		e->cTransform->velocityNormalized.normalize();
		}
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
			// TODO(CP)
			break;
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->wPressed = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->aPressed = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->sPressed = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->dPressed = true;
				break;
			}
		}
		break;
		case sf::Event::KeyReleased:
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->wPressed = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->aPressed = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->sPressed = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->dPressed = false;
				break;
			}
		}
		break;
		case sf::Event::MouseButtonPressed:
		{
			switch (event.key.code)
			{
			case sf::Mouse::Left:
				m_player->cInput->leftMouse = true;
				break;
			case sf::Mouse::Right:
				m_player->cInput->rightMouse = true;
				break;
			}
		}
		break;
		case sf::Event::MouseButtonReleased:
		{
			switch (event.key.code)
			{
			case sf::Mouse::Left:
				m_player->cInput->leftMouse = false;
				break;
			case sf::Mouse::Right:
				m_player->cInput->rightMouse = false;
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
	// Each entity has shape component
	for (auto& e : m_entityManager.getEntities()) {
		m_window.draw(e->cShape->shape);
	}
}

void Game::sEnemySpawner()
{

}

void Game::sCollision()
{
}
