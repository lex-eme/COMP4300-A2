#include <iostream>
#include <fstream>

#include "Game.h"

Game::Game(const std::string& config)
{
	init(config);
}

void Game::run()
{
	// TODO: add pause functionality in here
	//		some systems should function while paused (rendering)
	//		some systems shouldn't (movement / input)

	while (m_Running)
	{
		// update the entity manager
		m_Entities.update();

		// required update call to imgui
		ImGui::SFML::Update(m_Window, m_DeltaClock.restart());

		sUserInput();
		sEnemySpawner();
		sMovement();
		sCollision();
		sGUI();
		sRender();

		// increment the current frame
		// may need to be moved when pause implemented
		m_CurrentFrame += 1;
	}

	ImGui::SFML::Shutdown();
}

void Game::init(const std::string& path)
{
	std::ifstream fin("src/config.txt");
	std::string temp;

	while (fin >> temp)
	{
		if (temp == "Window")
		{
			fin >> m_Resolution.x >> m_Resolution.y;
		}
		else if (temp == "Fonts")
		{
			std::string path;
			unsigned int charSize = 0;
			unsigned char r = 0, g = 0, b = 0;

			fin >> path >> charSize >> r >> g >> b;

			if (!m_Font.loadFromFile(path))
			{
				std::cerr << "Could not load font." << std::endl;
				exit(-1);
			}

			m_Text.setCharacterSize(charSize);
			m_Text.setFillColor(sf::Color(r, g, b));
		}
		else if (temp == "Player")
		{
			fin >> m_PlayerConfig.SR >> m_PlayerConfig.CR >> m_PlayerConfig.S
				>> m_PlayerConfig.FR >> m_PlayerConfig.FG >> m_PlayerConfig.FB
				>> m_PlayerConfig.OR >> m_PlayerConfig.OG >> m_PlayerConfig.OB
				>> m_PlayerConfig.OT >> m_PlayerConfig.V;
		}
		else if (temp == "Enemy")
		{
			fin >> m_EnemyConfig.SR >> m_EnemyConfig.CR >> m_EnemyConfig.SMIN
				>> m_EnemyConfig.SMAX >> m_EnemyConfig.OR >> m_EnemyConfig.OG
				>> m_EnemyConfig.OB >> m_EnemyConfig.OT >> m_EnemyConfig.VMIN
				>> m_EnemyConfig.VMAX >> m_EnemyConfig.L >> m_EnemyConfig.SI;
		}
		else if (temp == "Bullet")
		{
			fin >> m_BulletConfig.SR >> m_BulletConfig.CR >> m_BulletConfig.S
				>> m_BulletConfig.FR >> m_BulletConfig.FG >> m_BulletConfig.FB
				>> m_BulletConfig.OR >> m_BulletConfig.OG >> m_BulletConfig.OB
				>> m_BulletConfig.OT >> m_BulletConfig.V >> m_BulletConfig.L;
		}
	}

	m_Window.create(sf::VideoMode(m_Resolution.x, m_Resolution.y), "Assignment 2");
	m_Window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(m_Window);

	srand(time(nullptr));

	spawnPlayer();
}

void Game::setPause(bool paused)
{
	m_Paused = paused;
}

void Game::spawnPlayer()
{
	// TODO: Finish adding all properties of the player with the correct values from th

	auto entity = m_Entities.addEntity("player");
	entity->cTransform = std::make_shared<CTransform>(Vec2(200, 200), Vec2(0, 0), 0.0f);
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(32.0f);

	// Since we want this Entity to be our player, set ou Game's player variable to be this entity
	// This goes slightly against the EntityManger paradigm, but we use the player so much it's worth it
	m_Player = entity;
}

void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with th m_EnemyConfig valirables
	//		the enemy must be spawned completely within the bounds of the window
	EnemyConfig& conf = m_EnemyConfig;
	float posX = (rand() % ((int)m_Resolution.x - 2 * conf.CR)) + conf.CR;
	float posY = (rand() % ((int)m_Resolution.y - 2 * conf.CR)) + conf.CR;
	int vertices = (rand() % (1 + conf.VMAX - conf.VMIN)) + conf.VMIN;

	float speed = (rand() % (1 + conf.SMAX - conf.SMIN)) + conf.SMIN;
	float angle = (rand() % 360);
	Vec2 velocity(1, 0);
	velocity.rotate(angle);
	velocity *= speed;

	sf::Uint8 r = rand() % 255;
	sf::Uint8 g = rand() % 255;
	sf::Uint8 b = rand() % 255;

	auto entity = m_Entities.addEntity("enemy");
	entity->cTransform = std::make_shared<CTransform>(Vec2(posX, posY), velocity, 0.0f);
	entity->cShape = std::make_shared<CShape>(conf.SR, vertices, sf::Color(r, g, b), sf::Color(conf.OR, conf.OG, conf.OB), 2.0f);
	entity->cCollision = std::make_shared<CCollision>(conf.CR);

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_CurrentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	// TODO: spawn small enemies at the location of the input e

	// when we create the smaller enemy, we have to read the values of the original enemy
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original , half the size
	// - small enemies are worth double points of the original enemy
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& taget)
{
	// TODO: implement the spawning of a bullet which travels toward target
	//		- bullet speed si given as a scalar speed
	//		- you must set the velocity by using formula in notes
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}

void Game::sMovement()
{
	// TODO: implement all entity movement in this function
	//		you should read the m_Player->cInput component to determine if the player is moving

	CInput input = *m_Player->cInput;
	Vec2 playerVel(0, 0);
	if (input.up)		playerVel -= Vec2(0, 1);
	if (input.down)		playerVel -= Vec2(0, -1);
	if (input.right)	playerVel += Vec2(1, 0);
	if (input.left)		playerVel += Vec2(-1, 0);
	playerVel.setMag(m_PlayerConfig.S);
	m_Player->cTransform->velocity = playerVel;

	for (auto e : m_Entities.getEntities())
	{
		if (e->cShape)
			e->cTransform->velocity.rotate(0.1f);
			e->cTransform->pos += e->cTransform->velocity;
	}
}

void Game::sLifespan()
{
	// TODO: implement all lifespan functionality
	
	// for all entities
	//		if entity has no lifespan component, skip it
	//		if entity has > 0 remaining lidespan, substract 1
	//		if it has lifespan and is alive
	//			scale its alpha channel properly
	//		if it has lifespan and its time is up
	//			destroy the entity
}

void Game::entitiesCheckBounds(const std::string& tag)
{
	for (auto e : m_Entities.getEntities(tag))
	{
		Vec2& pos = e->cTransform->pos;
		Vec2& vel = e->cTransform->velocity;
		float radius = e->cCollision->radius;

		if (pos.y - radius < 0.0f)
		{
			vel.y *= -1.0f;
			pos.y = radius;
		}
		else if (pos.y + radius > m_Resolution.y)
		{
			vel.y *= -1.0f;
			pos.y = m_Resolution.y - radius;
		}

		if (pos.x - radius < 0.0f)
		{
			vel.x *= -1.0f;
			pos.x = radius;
		}
		else if (pos.x + radius > m_Resolution.x)
		{
			vel.x *= -1.0f;
			pos.x = m_Resolution.x - radius;
		}
	}
}

void Game::sCollision()
{
	entitiesCheckBounds("enemy");
	entitiesCheckBounds("small enemy");

	Vec2& pos = m_Player->cTransform->pos;
	float radius = m_Player->cCollision->radius;

	if (pos.y - radius < 0.0f)
		pos.y = radius;
	if (pos.y + radius > m_Resolution.y)
		pos.y = m_Resolution.y - radius;
	if (pos.x - radius < 0.0f)
		pos.x = radius;
	if (pos.x + radius > m_Resolution.x)
		pos.x = m_Resolution.x - radius;
}

void Game::sEnemySpawner()
{
	// TODO: code whichi implements enemy spawning should go here
	int time = m_CurrentFrame - m_lastEnemySpawnTime;
	if (time > m_EnemyConfig.SI)
		spawnEnemy();
}

void Game::sGUI()
{
	//ImGui::ShowDemoWindow();
	ImGui::Begin("Geometry Wars");

	ImGui::Text("Stuff goes here!");

	ImGui::End();
}

void Game::sRender()
{
	m_Window.clear();

	for (auto e : m_Entities.getEntities())
	{
		if (e->cShape)
		{
			e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
			e->cTransform->angle += 1.0f;
			e->cShape->circle.setRotation(e->cTransform->angle);
			m_Window.draw(e->cShape->circle);
		}
	}

	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

void Game::sUserInput()
{
	// TODO: handle user input here
	//		note that you should only be setting the player's input component variables here
	//		you should not implement the player's movement logic here
	//		the movement system will read the variables you set in this function

	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
			m_Running = false;

		if (event.type == sf::Event::KeyPressed)
		{
			//std::cout << "Key pressed with code = " << event.key.code << std::endl;

			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_Running = false;
				break;
			case sf::Keyboard::P:
				setPause(!m_Paused);
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			// this line ignores mouse events if ImGui is the thing being clicked
			if (ImGui::GetIO().WantCaptureMouse) { continue; }

			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
				// call spawnBullet here
			}

			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
				// call spawnSpecialWeapon here
			}
		}
	}

	auto input = m_Player->cInput;
	input->up = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	input->down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	input->left = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
	input->right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}
