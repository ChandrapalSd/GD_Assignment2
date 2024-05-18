#include "Game.hpp"
#include "Vec2.hpp"
#include "Util.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

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
	m_font.loadFromFile(config.fontFilePath);
	const auto ws = m_window.getSize();

	m_entityManager.init({ "player" });
	m_player = m_entityManager.getEntities("player").front();
	m_player->cTransform = std::make_shared<CTransform>(Vec2((float)ws.x/2, (float)ws.y/2), config.playerSpeed, Vec2());
	m_player->cInput = std::make_shared<CInput>();
	m_player->cShape = std::make_shared<CShape>(config.playerSR, config.playerVertices);
	m_player->cShape->shape.setFillColor(sf::Color(config.playerFR, config.playerFG, config.playerFB, 100));
	m_player->cShape->shape.setOutlineColor(sf::Color(config.playerOR, config.playerOG, config.playerOB));
	m_player->cShape->shape.setOutlineThickness(config.playerOT);
	m_player->cCollision = std::make_shared<CCollision>(config.playerCR);

	m_player->cGun = std::make_shared<CGun>(200);

	m_player->cScore = std::make_shared<CScore>();
	m_player->cScore->scoreText.setFont(m_font);
	m_player->cScore->scoreText.setCharacterSize(config.fontSize);
	m_player->cScore->scoreText.setFillColor(sf::Color(config.fontR, config.fontG, config.fontB));
	m_player->cScore->scoreText.setPosition(3, 3);
	m_player->cScore->scoreText.setString("Score : 0");

	m_backTex.loadFromFile("res/bg.png");
	m_backTex.setRepeated(true);
	m_background.setTexture(m_backTex);
	m_background.scale(2, 2);
	m_background.setColor(sf::Color(255,255,255,100));

	srand((unsigned int)time(0));
}

void Game::update()
{
	m_entityManager.update();

	sUserInput();

	if (!m_paused) {
		if(gameState.EnableSMovement) sMovement();
		if (gameState.EnableSCollision) sCollision();
		if (gameState.EnableSScore) sScore();
		sRender();
		if (gameState.EnableSEnemySpawner) sEnemySpawner();
		if (gameState.EnableSPlayerWeapon) sPlayerWeapon();
		if (gameState.EnableSLifetimeManagement) sLifetimeManagement();
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
		e->cShape->shape.rotate(speed/2);
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
			if (ImGui::GetIO().WantCaptureMouse) continue;
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
			if (ImGui::GetIO().WantCaptureMouse) continue;
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

void Game::DrawImGuiUI(sf::Time deltaTime)
{
	ImGui::SFML::Update(m_window, deltaTime);

	const auto ws = m_window.getSize();


	ImGui::Begin("Debug panel");
	ImGui::Text("Press P to Pause / Resume");
	std::string titleEntitiesHeader = "Entities (" + std::to_string(m_entityManager.getEntities().size()) + ") ###EntitiesHeader";
	if (ImGui::CollapsingHeader(titleEntitiesHeader.c_str()))
	{
		for (auto& e : m_entityManager.getEntities())
		{
			const sf::Color clr = e->cShape->shape.getFillColor();
			const ImVec4 btnClr{ clr.r / 255.0f,clr.g / 255.0f,clr.b / 255.0f,clr.a / 255.0f };
			const std::string btnDes_id = "D##EntityBtnKill" + std::to_string(e->id());

			// Destroy button
			ImGui::PushStyleColor(ImGuiCol_Button, btnClr);
			if (ImGui::Button(btnDes_id.c_str()) && e != m_player)
				e->destroy();
			ImGui::PopStyleColor(1);

			ImGui::SameLine();			
			uiForEntity(e);
		}
	}

	if (ImGui::CollapsingHeader("Systems")) {
		// ImGui::Checkbox("SUserInput", &gameState.EnableSUserInput);
		ImGui::Checkbox("SMovement", &gameState.EnableSMovement);
		ImGui::Checkbox("SCollision", &gameState.EnableSCollision);
		ImGui::Checkbox("SScore", &gameState.EnableSScore);
		ImGui::Checkbox("SRender", &gameState.EnableSRender);
		ImGui::Checkbox("SEnemySpawner", &gameState.EnableSEnemySpawner);
		ImGui::Checkbox("SPlayerWeapon", &gameState.EnableSPlayerWeapon);
		ImGui::Checkbox("SLifetimeManagement", &gameState.EnableSLifetimeManagement);
	}

	if (ImGui::CollapsingHeader("Debug Mouse")) {
		auto mpos = (sf::Mouse::getPosition(m_window));
		auto mposMapped = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
		std::stringstream out;
		out << "Pos         : " << mpos.x << ", " << mpos.y << "\n"
			<< "Pos Mapped  : " << mposMapped.x << ", " << mposMapped.y << "\n"
			<< "Window Size : " << ws.x << ", " << ws.y;

		ImGui::Text(out.str().c_str());
	}
	ImGui::End();

#ifdef DEMO
	ImGui::ShowDemoWindow();
#endif // DEMO
	ImGui::SFML::Render(m_window);
}

void Game::sRender()
{
	sf::Time deltaTime = m_deltaClock.restart();
	m_window.clear(sf::Color::Black);

	if (gameState.EnableSRender)
	{
		m_window.draw(m_background);

		// Each entity has shape component
		for (auto& e : m_entityManager.getEntities()) {
			m_window.draw(e->cShape->shape);
		}
		m_window.draw(m_player->cScore->scoreText);
	}

	DrawImGuiUI(deltaTime);
	m_window.display();
}

void Game::sEnemySpawner()
{
	const auto ws = m_window.getSize();
	static uint32_t framesPast = 0;
	if (framesPast++ > config.enemySI)
	{
		const uint8_t cMin = 10, cMax = 255;
		const float radius = config.enemySR, radiusCollision = config.enemyCR;

		int pCount = randInRange(config.enemyVertMin, config.enemyVertMax);
		Vec2 pos(randInRange(radius, ws.x - radius), randInRange(radius, ws.y - radius));
		sf::Color clr(randInRange(cMin, cMax), randInRange(cMin, cMax), randInRange(cMin, cMax));
		float speed = randInRange(config.enemySMin, config.enemySMax);
		size_t verticesCount = randInRange(config.enemyVertMin, config.enemyVertMax);

		std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy");
		e->cTransform = std::make_shared<CTransform>(pos, speed);
		e->cShape = std::make_shared<CShape>(radius, verticesCount);
		e->cShape->shape.setFillColor(clr);
		e->cShape->shape.setOutlineColor(sf::Color(config.enemyOR, config.enemyOG, config.enemyOB));
		e->cShape->shape.setOutlineThickness(config.enemyOT);
		e->cCollision = std::make_shared<CCollision>(radiusCollision);
		framesPast = 0;
	}
}

void Game::sPlayerWeapon()
{
	auto currTime = std::chrono::high_resolution_clock::now();
	if (m_player->cInput->leftMouse && (currTime - m_player->cGun->lastSpawned).count() > m_player->cGun->coolDownTime) {
		m_player->cGun->lastSpawned = currTime;
		const auto ppos = m_player->cTransform->pos;

		auto mpos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
		Vec2 towards(Vec2(mpos.x, mpos.y) - ppos);
		towards.normalize();

		const float radius = config.bulletSR, radiusCollision = config.bulletCR;
		const float speed = config.bulletSpeed;

		auto bullet = m_entityManager.addEntity("bullet");
		bullet->cTransform = std::make_shared<CTransform>(ppos, speed, towards);
		bullet->cShape = std::make_shared<CShape>(radius, config.bulletVertices);
		bullet->cShape->shape.setFillColor(sf::Color(config.bulletFR, config.bulletFG, config.bulletFB));
		bullet->cShape->shape.setOutlineColor(sf::Color(config.bulletOR, config.bulletOG, config.bulletOB));
		bullet->cShape->shape.setOutlineThickness(config.bulletOT);

		bullet->cCollision = std::make_shared<CCollision>(radiusCollision);

		bullet->cLifespan = std::make_shared<CLifespan>(config.bulletLifespan, config.bulletLifespan/1.5f);
	}

}

void Game::sLifetimeManagement()
{
	const auto ws = m_window.getSize();

	for (auto& b : m_entityManager.getEntities("bullet")) {
		const auto& pos = b->cTransform->pos;
		size_t& fl = b->cLifespan->framesLeft;
		size_t maxFrames = b->cLifespan->startDisapearingAtFrames;

		uint8_t minOpacity = 50, maxOpacity = 255;
		uint8_t opacity = lerp(minOpacity, maxOpacity, (float)fl / maxFrames);
		sf::Color fcolor = b->cShape->shape.getFillColor();
		sf::Color ocolor = b->cShape->shape.getOutlineColor();
		fcolor.a = opacity, ocolor.a = opacity;
		b->cShape->shape.setFillColor(fcolor);
		b->cShape->shape.setOutlineColor(ocolor);

		if (fl==0 || pos.x < 0 || pos.x > ws.x || pos.y < 0 || pos.y> ws.y) {
			b->destroy();
		}

		fl--;
	}
	for (auto& b : m_entityManager.getEntities("enemy-fragments")) {
		const auto& pos = b->cTransform->pos;
		size_t& fl = b->cLifespan->framesLeft;
		size_t maxFrames = b->cLifespan->startDisapearingAtFrames;

		uint8_t minOpacity = 40, maxOpacity = 255;
		uint8_t opacity = lerp(minOpacity, maxOpacity, (float)fl / maxFrames);
		sf::Color fcolor = b->cShape->shape.getFillColor();
		sf::Color ocolor = b->cShape->shape.getOutlineColor();
		fcolor.a = opacity, ocolor.a = opacity;
		b->cShape->shape.setFillColor(fcolor);
		b->cShape->shape.setOutlineColor(ocolor);

		if (fl==0 || pos.x < 0 || pos.x > ws.x || pos.y < 0 || pos.y> ws.y) {
			b->destroy();
		}

		fl--;
	}
}

bool areColliding(Vec2 apos, Vec2 bpos, float arad, float brad)
{
	return Vec2::dist(apos, bpos) < arad + brad;
}

void Game::shootEnemy(std::shared_ptr<Entity>& e)
{
	e->destroy();

	Vec2 center = e->cTransform->pos;
	float speed = e->cTransform->speed;
	size_t vertCount = e->cShape->pointCount;
	float radius = e->cShape->radius;
	float radiusFrag = radius / 2;
	sf::Color clr = e->cShape->shape.getFillColor();

	for (int i = 0; i < vertCount; i++)
	{		
		const float angle = (TAU/vertCount)*i;
		Vec2 pos = Vec2(std::cosf(angle), std::sinf(angle)) * radiusFrag + center;

		std::shared_ptr<Entity> e = m_entityManager.addEntity("enemy-fragments");
		e->cTransform = std::make_shared<CTransform>(pos, speed/2, Vec2::normalize(pos-center));
		
		e->cShape = std::make_shared<CShape>(radiusFrag, vertCount);
		e->cShape->shape.setFillColor(clr);
		e->cShape->shape.setOutlineColor(sf::Color::White);
		e->cShape->shape.setOutlineThickness(2);

		e->cLifespan = std::make_shared<CLifespan>(150, 120);
	}
}

void Game::sCollision()
{
	const Vec2 ppos = m_player->cTransform->pos;
	const float prad = m_player->cShape->radius;
	for (auto& e : m_entityManager.getEntities("enemy")) 
	{
		/*if (Vec2::dist(ppos, e->cTransform->pos) < prad + e->cShape->radius)*/
		if(areColliding(e->cTransform->pos, ppos, e->cShape->radius, prad))
		{
			e->cTransform->velocityNormalized *= -1;
			// TODO(CP) : properly push out enemy out of player
			e->cTransform->pos += e->cTransform->velocityNormalized * ((prad + e->cShape->radius) - (e->cTransform->pos - ppos).length());
		}
			

		for (auto& b : m_entityManager.getEntities("bullet"))
		{
			if (areColliding(e->cTransform->pos, b->cTransform->pos, e->cShape->radius, b->cCollision->radius))
			{
				b->destroy();
				shootEnemy(e);
			}
		}
	}
}


void Game::sScore()
{
	m_player->cScore->scoreText.setString("Score : " + std::to_string(m_player->cScore->score++/25));
}
