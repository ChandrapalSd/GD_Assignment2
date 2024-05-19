#pragma once
#include "EntityManager.hpp"
#include "Config.hpp"

class Game;

class Scene
{
protected:
	Scene(Game* gameEngine)
		: gameEngine(gameEngine){}
public:
	virtual void update(sf::Time deltaTime) = 0;
	//virtual void sDoAction() = 0;
	virtual void sRender(sf::Time deltaTime) = 0;

	/*void simulate(int);
	void doAction(void);
	void registerAction(void);*/
public:
    Game* gameEngine = 0;
	EntityManager m_entityManager;
	int currentFrame = 0;
	std::map<int, std::string> actionMap;
	bool paused = false;
	bool hasEnded = false;
};


class ScenePlay : public Scene
{
	struct SysStates {
		bool EnableSUserInput = true;
		bool EnableSMovement = true;
		bool EnableSCollision = true;
		bool EnableSScore = true;
		bool EnableSRender = true;
		bool EnableSEnemySpawner = true;
		bool EnableSPlayerWeapon = true;
		bool EnableSLifetimeManagement = true;
	};


	public:
		ScenePlay(const std::string configFilePath, Game* gameEngine);

		void init();
		virtual void update(sf::Time deltaTime);

		void DrawImGuiUI(sf::Time deltaTime);

		// Systems
		void sMovement();
		void sCollision();
		void sScore();
		virtual void sRender(sf::Time deltaTime);
		void sEnemySpawner();
		void sPlayerWeapon();
		void sLifetimeManagement();

		void shootEnemy(std::shared_ptr<Entity>& e);


	private:
		const Config config;
		SysStates gameState;
		std::shared_ptr<Entity> m_player;

		sf::Font m_font;
		sf::Texture m_backTex;
		sf::Sprite m_background;
};
