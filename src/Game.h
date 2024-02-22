#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow		m_Window;
	EntityManager			m_Entities;
	sf::Font				m_Font;
	sf::Text				m_Text;
	PlayerConfig			m_PlayerConfig;
	EnemyConfig				m_EnemyConfig;
	BulletConfig			m_BulletConfig;
	sf::Clock				m_DeltaClock;
	Vec2					m_Resolution;
	int						m_Score;
	int						m_CurrentFrame = 0;
	int						m_lastEnemySpawnTime = 0;
	bool					m_Running = true;
	bool					m_Paused = false;
	std::shared_ptr<Entity>	m_Player;

public:
	Game(const std::string& config);
	void run();

private:
	void init(const std::string& path);
	void setPause(bool paused);
	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> e);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& taget);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

	void sMovement();
	void sLifespan();
	void sCollision();
	void sEnemySpawner();
	void sGUI();
	void sRender();
	void sUserInput();

	void entitiesCheckBounds(const std::string& tag);
};

