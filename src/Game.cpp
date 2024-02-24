#include <iostream>
#include <fstream>
#include <sstream>

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

		if (!m_Paused)
		{
			if (m_ActiveSystems.lifespan)
				sLifespan();
			if (m_ActiveSystems.enelySpawner)
				sEnemySpawner();
			if (m_ActiveSystems.movement)
				sMovement();
			if (m_ActiveSystems.collision)
				sCollision();
		}

		if (m_ActiveSystems.input)
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
			int r = 0, g = 0, b = 0;

			fin >> path >> charSize >> r >> g >> b;

			if (!m_Font.loadFromFile(path))
			{
				std::cerr << "Could not load font." << std::endl;
				exit(-1);
			}

			m_Text.setCharacterSize(charSize);
			m_Text.setFillColor(sf::Color(r, g, b));
			m_Text.setPosition(20.0f, 20.0f);
			m_Text.setFont(m_Font);
			m_Text.setString("Score: 0");
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

	m_Window.create(sf::VideoMode((int)m_Resolution.x, (int)m_Resolution.y), "Assignment 2");
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
	auto entity = m_Entities.addEntity("player");
	entity->cTransform = std::make_shared<CTransform>(Vec2(m_Resolution.x / 2, m_Resolution.y / 2), Vec2(0, 0), 0.0f);
	entity->cShape = std::make_shared<CShape>((float)m_PlayerConfig.SR, m_PlayerConfig.V, sf::Color(m_PlayerConfig.FR, m_PlayerConfig.FG, m_PlayerConfig.FB), sf::Color(m_PlayerConfig.OR, m_PlayerConfig.OG, m_PlayerConfig.OB), (float)m_PlayerConfig.OT);
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>((float)m_PlayerConfig.CR);
	m_Player = entity;
}

void Game::spawnEnemy()
{
	EnemyConfig& conf = m_EnemyConfig;
	int posX = (rand() % ((int)m_Resolution.x - 2 * conf.CR)) + conf.CR;
	int posY = (rand() % ((int)m_Resolution.y - 2 * conf.CR)) + conf.CR;
	int vertices = (rand() % (1 + conf.VMAX - conf.VMIN)) + conf.VMIN;

	float speed = (float)(rand() % (1 + conf.SMAX - conf.SMIN)) + conf.SMIN;
	float angle = (float)(rand() % 360);
	Vec2 velocity(1, 0);
	velocity.rotate(angle);
	velocity *= speed;

	sf::Uint8 r = rand() % 255;
	sf::Uint8 g = rand() % 255;
	sf::Uint8 b = rand() % 255;

	auto entity = m_Entities.addEntity("enemy");
	entity->cTransform = std::make_shared<CTransform>(Vec2(posX, posY), velocity, 0.0f);
	entity->cShape = std::make_shared<CShape>((float)conf.SR, vertices, sf::Color(r, g, b), sf::Color(conf.OR, conf.OG, conf.OB), (float)conf.OT);
	entity->cCollision = std::make_shared<CCollision>((float)conf.CR);
	entity->cScore = std::make_shared<CScore>(vertices * 100);

	m_lastEnemySpawnTime = m_CurrentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{
	int pointCount = e->cShape->circle.getPointCount();
	Vec2 pos = e->cTransform->pos;
	float angle = 360.0f / pointCount;
	Vec2 vel = e->cTransform->velocity;
	float radius = e->cShape->circle.getRadius() / 2;

	for (int i = 0; i < pointCount; i++)
	{
		auto se = m_Entities.addEntity("small enemy");
		se->cTransform = std::make_shared<CTransform>(pos, vel.rotate(angle), 0.0f);
		se->cShape = std::make_shared<CShape>(radius, pointCount, e->cShape->circle.getFillColor(), e->cShape->circle.getOutlineColor(), e->cShape->circle.getOutlineThickness());
		se->cCollision = std::make_shared<CCollision>(radius);
		se->cLifeSpan = std::make_shared<CLifeSpan>(m_EnemyConfig.L);
		se->cScore = std::make_shared<CScore>(e->cScore->score * 2);
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	Vec2 pos = entity->cTransform->pos;
	Vec2 vel = target - pos;
	vel.normalize();
	vel *= m_BulletConfig.S;

	auto e = m_Entities.addEntity("bullet");
	e->cTransform = std::make_shared<CTransform>(pos, vel, 0.0f);
	e->cShape = std::make_shared<CShape>((float)m_BulletConfig.SR, m_BulletConfig.V, sf::Color(m_BulletConfig.FR, m_BulletConfig.FG, m_BulletConfig.FB), sf::Color(m_BulletConfig.OR, m_BulletConfig.OG, m_BulletConfig.OB), 1.0f);
	e->cCollision = std::make_shared<CCollision>((float)m_BulletConfig.CR);
	e->cLifeSpan = std::make_shared<CLifeSpan>(m_BulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: implement your own special weapon
}

void Game::sMovement()
{
	CInput input = *m_Player->cInput;
	Vec2 playerVel(0, 0);
	if (input.up)		playerVel -= Vec2(0, 1);
	if (input.down)		playerVel += Vec2(0, 1);
	if (input.right)	playerVel += Vec2(1, 0);
	if (input.left)		playerVel += Vec2(-1, 0);
	playerVel.setMag(m_PlayerConfig.S);
	m_Player->cTransform->velocity = playerVel;

	for (auto e : m_Entities.getEntities())
	{
		if (e->cShape)
			e->cTransform->pos += e->cTransform->velocity;
	}
}

static void setAlpha(std::shared_ptr<Entity> e)
{
	float ratio = (float)e->cLifeSpan->remaining / e->cLifeSpan->total;
	sf::Color fillColor = e->cShape->circle.getFillColor();
	sf::Color outlineColor = e->cShape->circle.getOutlineColor();
	fillColor.a = ratio * 255;
	outlineColor.a = ratio * 255;
	e->cShape->circle.setFillColor(fillColor);
	e->cShape->circle.setOutlineColor(outlineColor);
}

void Game::sLifespan()
{
	for (auto e : m_Entities.getEntities())
	{
		if (e->cLifeSpan)
		{
			CLifeSpan& ls = *e->cLifeSpan;
			ls.remaining -= 1;

			if (ls.remaining <= 0)
				e->destroy();
			else
				setAlpha(e);
		}
	}
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

static bool collides(const Vec2& a, const Vec2& b, float len)
{
	float dist = a.dist(b);
	return dist < len;
}

void Game::deleteEnemy(std::shared_ptr<Entity> e)
{
	m_Score += e->cScore->score;
	std::stringstream ss;
	ss << "Score: " << m_Score;

	m_Text.setString(ss.str());
	e->destroy();
}

void Game::sCollision()
{
	entitiesCheckBounds("enemy");

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

	for (auto e : m_Entities.getEntities("enemy"))
	{

		if (collides(pos, e->cTransform->pos, radius + e->cCollision->radius))
		{
			spawnSmallEnemies(e);
			e->destroy();
			m_Player->destroy();
			spawnPlayer();
		}

		for (auto bullet : m_Entities.getEntities("bullet"))
		{
			if (collides(e->cTransform->pos, bullet->cTransform->pos, e->cCollision->radius + bullet->cCollision->radius))
			{
				spawnSmallEnemies(e);
				deleteEnemy(e);
				bullet->destroy();
			}
		}
	}

	for (auto e : m_Entities.getEntities("small enemy"))
	{

		if (collides(pos, e->cTransform->pos, radius + e->cCollision->radius))
		{
			e->destroy();
			m_Player->destroy();
			spawnPlayer();
		}

		for (auto bullet : m_Entities.getEntities("bullet"))
		{
			if (collides(e->cTransform->pos, bullet->cTransform->pos, e->cCollision->radius + bullet->cCollision->radius))
			{
				deleteEnemy(e);
				bullet->destroy();
			}
		}
	}
}

void Game::sEnemySpawner()
{
	int time = m_CurrentFrame - m_lastEnemySpawnTime;
	if (time > m_EnemyConfig.SI)
		spawnEnemy();
}

static void entityGUI(std::shared_ptr<Entity> e)
{
	std::stringstream ss;
	ss << "D##" << e->id();
	if (ImGui::Button(ss.str().c_str())) {
		e->destroy();
	}
	ImGui::SameLine();
	ImGui::Text("%i", e->id());
	ImGui::SameLine();
	ImGui::Text(e->tag().c_str());
	ImGui::SameLine();
	ImGui::Text("(%f,%f)", e->cTransform->pos.x, e->cTransform->pos.y);
}

void Game::sGUI()
{
	ImGui::Begin("Geometry Wars");
	ImGui::SetWindowFontScale(2.0f);

	if (ImGui::BeginTabBar("Tabs", 0))
	{
		if (ImGui::BeginTabItem("Systems"))
		{
			ImGui::Checkbox("Movement", &m_ActiveSystems.movement);
			ImGui::Checkbox("Lifespan", &m_ActiveSystems.lifespan);
			ImGui::Checkbox("Collision", &m_ActiveSystems.collision);
			ImGui::Checkbox("Spawner", &m_ActiveSystems.enelySpawner);
			ImGui::SameLine();
			if (ImGui::Button("Spawn"))
				spawnEnemy();
			ImGui::Checkbox("Input", &m_ActiveSystems.input);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Entities"))
		{
			if (ImGui::CollapsingHeader("All entities", ImGuiTreeNodeFlags_NoTreePushOnOpen))
			{
				for (auto e : m_Entities.getEntities())
				{
					entityGUI(e);
				}
			}

			if (ImGui::CollapsingHeader("By tag"))
			{
				for (auto& [tag, entityVec] : m_Entities.getEntityMap())
				{
					if (ImGui::CollapsingHeader(tag.c_str()))
					{
						for (auto e : entityVec)
						{
							entityGUI(e);
						}
					}
				}
			}

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
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

	m_Window.draw(m_Text);

	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

void Game::sUserInput()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed)
			m_Running = false;

		if (event.type == sf::Event::KeyPressed)
		{
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
			// ignores mouse events if ImGui is the thing being clicked
			if (ImGui::GetIO().WantCaptureMouse) { continue; }

			if (!m_Paused)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					spawnBullet(m_Player, Vec2(event.mouseButton.x, event.mouseButton.y));
				}

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
					spawnSpecialWeapon(m_Player);
				}
			}
		}
	}

	CInput& input = *m_Player->cInput;
	input.up = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	input.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	input.left = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
	input.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
}
