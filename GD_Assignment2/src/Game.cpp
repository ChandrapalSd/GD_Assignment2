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
	m_font.loadFromFile("fonts/KillerTech.ttf");
	const auto ws = m_window.getSize();

	m_entityManager.init({ "player" });
	m_player = m_entityManager.getEntities("player").front();
	m_player->cTransform = std::make_shared<CTransform>(Vec2((float)ws.x/2, (float)ws.y/2), 5, Vec2());
	m_player->cInput = std::make_shared<CInput>();
	m_player->cShape = std::make_shared<CShape>(16, 6);
	m_player->cShape->shape.setFillColor(sf::Color::Transparent);
	m_player->cShape->shape.setOutlineColor(sf::Color::Red);
	m_player->cShape->shape.setOutlineThickness(3);
	m_player->cCollision = std::make_shared<CCollision>(10);

	m_player->cGun = std::make_shared<CGun>();

	m_player->cScore = std::make_shared<CScore>();
	m_player->cScore->scoreText.setFont(m_font);
	m_player->cScore->scoreText.setCharacterSize(20);
	m_player->cScore->scoreText.setFillColor(sf::Color::White);
	m_player->cScore->scoreText.setPosition(3, 3);
	m_player->cScore->scoreText.setString("Score : 0");

	m_backTex.loadFromFile("res/bg.png");
	m_backTex.setRepeated(true);
	m_background.setTexture(m_backTex);
	m_background.scale(2, 2);
	m_background.setColor(sf::Color(255,255,255,100));

	srand((unsigned int)time(0));
	for (int i = 0; i < 10; i++)
	{
		const float rMin = 10.0, rMax = 50.0;
		const uint8_t cMin = 10, cMax = 255;
		float radius = randInRange(rMin, rMax);
		int pCount = randInRange<int>(3, 21);
		Vec2 pos(randInRange(radius, ws.x-radius), randInRange(radius, ws.y-radius));
		sf::Color clr(randInRange(cMin, cMax), randInRange(cMin, cMax), randInRange(cMin, cMax));

		std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy");
		e->cTransform = std::make_shared<CTransform>(pos, randInRange(4.0f, 7.0f));
		e->cShape = std::make_shared<CShape>(radius, pCount);
		e->cShape->shape.setFillColor(clr);
		e->cShape->shape.setOutlineColor(sf::Color::White);
		e->cShape->shape.setOutlineThickness(2);
		e->cCollision = std::make_shared<CCollision>(radius);
	}
}

void Game::update()
{
	m_entityManager.update();

	sUserInput();

	if (!m_paused) {
		sMovement();
		sCollision();
		sScore();
		sRender();
		sEnemySpawner();
		sPlayerWeapon();
	}
}

void Game::run()
{
	while (isRunning()) {
		update();
	}
}

void Game::sMovement()
{
	const auto& windowSize = m_window.getSize();

	m_player->cTransform->velocityNormalized.x = m_player->cInput->dPressed ? 1 : m_player->cInput->aPressed ? -1 : 0;
	m_player->cTransform->velocityNormalized.y = m_player->cInput->sPressed ? 1 : m_player->cInput->wPressed ? -1 : 0;
	m_player->cTransform->velocityNormalized.normalize();

	// Each entity has transform component
	for (auto& e : m_entityManager.getEntities()) {
		float speed = e->cTransform->speed;
		e->cTransform->pos += (e->cTransform->velocityNormalized * speed ); // TODO(CP) : Multiply with deltatime?
		sf::Vector2f newPos((float)e->cTransform->pos.x, (float)e->cTransform->pos.y);
		e->cShape->shape.setPosition(newPos);
		e->cShape->shape.rotate(speed);
	}

	for (auto& e : m_entityManager.getEntities("enemy")) {

		const auto& pos = e->cTransform->pos;
		const float radius = e->cShape->radius;

		// TODO(CP) : Clamp pos to window
		if (pos.x - radius <= 0 || pos.x + radius >= windowSize.x) {
			e->cTransform->velocityNormalized.x *= -1;
		}
		if (pos.y - radius <= 0 || pos.y + radius >= windowSize.y) {
			e->cTransform->velocityNormalized.y *= -1;
		}
		e->cTransform->velocityNormalized.normalize();
	}

	Vec2& ppos = m_player->cTransform->pos;

	// Clamp player to window
	const float pradius = m_player->cShape->radius;
	if (ppos.x - pradius < 0) ppos.x = pradius;
	else if (ppos.x + pradius > windowSize.x) ppos.x = windowSize.x - pradius;
	if (ppos.y - pradius< 0) ppos.y = pradius;
	else if (ppos.y + pradius > windowSize.y) ppos.y = windowSize.y-pradius;

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
			const sf::Vector2u ws = m_window.getSize();
			sf::View view = m_window.getView();
			view.setSize(ws.x, ws.y);
			view.setCenter(ws.x / 2, ws.y / 2);
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
				m_paused = !m_paused;
				break;
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

static void uiForEntity(const std::shared_ptr<Entity> const& e)
{
	const std::string tag(e->tag());
	const std::string id(std::to_string(e->id()));
	const std::string label = "Entity: " + tag + " " + id;

	if (ImGui::CollapsingHeader(label.c_str())) {
		const std::string labelPos = "Pos##"+id;
		const std::string labelSpeed = "Speed##"+id;
		const std::string textRadius = "Radius : "+ std::to_string(e->cShape->radius);
		
		ImGui::DragFloat2(labelPos.c_str(), &e->cTransform->pos.x);
		ImGui::DragFloat(labelSpeed.c_str(), &e->cTransform->speed);
		ImGui::Text(textRadius.c_str());
	}
}

void Game::sRender()
{
	m_window.clear(sf::Color::Black);
	m_window.draw(m_background);
	
	ImGui::SFML::Update(m_window, m_deltaClock.restart());

	// Each entity has shape component
	for (auto& e : m_entityManager.getEntities()) {
		m_window.draw(e->cShape->shape);
	}

	ImGui::Begin("Debug panel");
	ImGui::Text("Press P to Pause / Resume");
	if (ImGui::CollapsingHeader("Entities"))
	{
		for (auto& e : m_entityManager.getEntities())
		{
			uiForEntity(e);
		}
	}
	ImGui::End();

#ifdef DEMO
	ImGui::ShowDemoWindow();
#endif // DEMO
	m_window.draw(m_player->cScore->scoreText);
	ImGui::SFML::Render(m_window);
	m_window.display();
}

void Game::sEnemySpawner()
{

}

void Game::sPlayerWeapon()
{
	auto currTime = std::chrono::high_resolution_clock::now();
	if (m_player->cInput->leftMouse && (currTime - m_player->cGun->lastSpawned).count() > m_player->cGun->coolDownTime) {
		m_player->cGun->lastSpawned = currTime;
		const auto ppos = m_player->cTransform->pos;

		auto bullet = m_entityManager.addEntity("bullet");
		// TODO(CP) : bullet towards mouse
		bullet->cTransform = std::make_shared<CTransform>(ppos, 5);
		bullet->cShape = std::make_shared<CShape>(3);
		bullet->cShape->shape.setFillColor(sf::Color::Black);
		bullet->cShape->shape.setOutlineColor(sf::Color::Red);
		bullet->cShape->shape.setOutlineThickness(1);

		static uint32_t bulletCount = 0;
		std::cout << "Bullet Count : " << std::to_string(++bulletCount) << std::endl;
	}
}

void Game::sCollision()
{
	const Vec2 ppos = m_player->cTransform->pos;
	const float prad = m_player->cShape->radius;
	static uint32_t collisionCount = 0;
	for (auto& e : m_entityManager.getEntities("enemy")) 
	{
		if (Vec2::dist(ppos, e->cTransform->pos) < prad + e->cShape->radius)
		{
			std::cout << "Collided " << ++collisionCount << " Times" << std::endl;
			e->cTransform->velocityNormalized *= -1;
		}
	}
}

void Game::sScore()
{
	m_player->cScore->scoreText.setString("Score : " + std::to_string(m_player->cScore->score++/25));
}
