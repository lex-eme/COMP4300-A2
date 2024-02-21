#include <iostream>

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

		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
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
	// TODO: read in config file here
	//		use the premade PlayerConfig, EnemyConfig, BulletConfig variables

	// set up default window parameters
	m_Window.create(sf::VideoMode(1280, 720), "Assignment 2");
	m_Window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(m_Window);

	spawnPlayer();
}

void Game::setPause(bool paused)
{
	// TODO
}

void Game::spawnPlayer()
{
	// TOFO: Finish adding all properties of the player with the correct values from th

	// We create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so we use 'auto' to save typing
	auto entity = m_Entities.addEntity("player");

	// Give this entity a Transform so it spawns at (200,200) with velocity (1,1) and angle 0
	entity->cTransform = std::make_shared<CTransform>(Vec2(200, 200), Vec2(1, 1), 0.0f);

	// The entity's shape will have radius 32, 8 sides, dark grey fill, and red outiline of thickness 4
	entity->cShape = std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

	// Add an input component to the player so that we can use inputs
	entity->cInput = std::make_shared<CInput>();

	// Since we want this Entity to be our player, set ou Game's player variable to be this entity
	// This goes slightly against the EntityManger paradigm, but we use the player so much it's worth it
	m_Player = entity; // not added to the EntityManger yet!
}

void Game::spawnEnemy()
{
	// TODO: make sure the enemy is spawned properly with th m_EnemyConfig valirables
	//		the enemy must be spawned completely within the bounds of the window

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

	// Sample movement
	m_Player->cTransform->pos.x += m_Player->cTransform->velocity.x;
	m_Player->cTransform->pos.y += m_Player->cTransform->velocity.y;
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

void Game::sCollision()
{
	// TODO: implement all proper collisions between entities
	//		be sure to use the collision radius, NOT the shape raidus
}

void Game::sEnemySpawner()
{
	// TODO: code whichi implements enemy spawning should go here
}

void Game::sGUI()
{
	ImGui::Begin("Geometry Wars");

	ImGui::Text("Stuff goes here!");

	ImGui::End();
}

void Game::sRender()
{
	// TODO: change the code below to draw ALL of the entities
	//		sample drawing of the player Entity that we have created
	m_Window.clear();

	m_Player->cShape->circle.setPosition(m_Player->cTransform->pos.x, m_Player->cTransform->pos.y);
	m_Player->cTransform->angle += 1.0f;
	m_Player->cShape->circle.setRotation(m_Player->cTransform->angle);

	m_Window.draw(m_Player->cShape->circle);

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
			std::cout << "Key pressed with code = " << event.key.code << std::endl;

			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_Running = false;
				break;
			case sf::Keyboard::Z:
				std::cout << "Z Key Pressed" << std::endl;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			std::cout << "Key released with code = " << event.key.code << std::endl;
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
}
