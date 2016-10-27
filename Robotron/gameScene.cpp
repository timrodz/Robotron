//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2016 Media Design School
//
// File Name : gameScene.cpp
// Description : Game scene implementation file - singleton game scene class that creates the models
// Mail : Mitchell.Currie@mediadesignschool.com
//

#include <iostream>
#include "glew.h"
#include "freeglut.h"

#include "gameScene.h"

GameScene* GameScene::s_pGameScene{ 0 };

GameScene::GameScene()
	//	: m_pClock(0)
{
	m_pClock = new CClock;
	m_pBullet = nullptr;
	m_pPlayer = nullptr;
}
GameScene::~GameScene()
{
	delete m_pClock;
	m_pClock = 0;
}

GameScene& GameScene::GetInstance()
{
	if (s_pGameScene == 0)
	{
		s_pGameScene = new GameScene();
	}

	return (*s_pGameScene);
}

void GameScene::AddCamera(Camera _camera)
{
	m_Camera = _camera;
}

void GameScene::AddEntity(Entity _entity)
{
	m_entities.push_back(_entity);
}

void GameScene::AddBullet(Entity* _entity)
{
	m_bullets.push_back(_entity);
}

void GameScene::AddText(TextLabel _text)
{
	m_textLabels.push_back(_text);
}

void GameScene::CheckBullets()
{	
	if (Entity::IsBulletFired())
	{
		m_pBullet = new Entity;
		m_pBullet->Initialise(BULLET, DOT, 6, m_Camera, m_pPlayer->GetModel()->GetPlayerPosition(), false, false);
		AddBullet(m_pBullet);
	}

	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		if ((*it)->ToDeleteBullet())
		{
			m_bulletsToDelete.push_back(*it);
		}
	}

	while (!m_bulletsToDelete.empty())
	{
		DeleteBullet(m_bulletsToDelete.back());
		m_bulletsToDelete.pop_back();
	}


	/*
	Entity function

	m_pBullet = this;
	if (m_pBullet->ToDeleteBullet())
	{
		delete m_pBullet;
	}
	
	*/
}

void GameScene::DeleteBullet(Entity* _bullet)
{
	_bullet = nullptr;
}

void GameScene::RenderEntities()
{
	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		if ((*it) != nullptr)
			(*it)->Render();
	}

	for (auto it = m_entities.begin(); it != m_entities.end(); it++)
	{		
		it->Render();	
	}
}

void GameScene::RenderText()
{
	for (auto it = m_textLabels.begin(); it != m_textLabels.end(); it++)
	{
		it->Render();
	}
}

void GameScene::CreateEntities()
{
	// Player Cube
	Entity Player;
	Player.Initialise(PLAYER, CUBE, 36, m_Camera, vec3(0.0f, 0.0f, 0.0f), true, false);
	AddEntity(Player);
	m_pPlayer = &Player;

	// Map Quad
	Entity Map;
	Map.Initialise(MAP, QUAD, 6, m_Camera, vec3(0.0f, 0.0f, 0.0f), false, false);
	AddEntity(Map);

	// Enemy Cubes
	Entity Enemy1;
	Enemy1.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(5.0f, 0.0f, 5.0f), false, true);
	AddEntity(Enemy1);

	Entity Enemy2;
	Enemy2.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(-5.0f, 0.0f, -5.0f), false, false);
	AddEntity(Enemy2);

	Entity Enemy3;
	Enemy3.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(5.0f, 0.0f, -5.0f), false, false);
	AddEntity(Enemy3);

	Entity Enemy4;
	Enemy4.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(-5.0f, 0.0f, 5.0f), false, false);
	AddEntity(Enemy4);

	Entity Enemy5;
	Enemy5.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(1.0f, 0.0f, 1.0f), false, false);
	AddEntity(Enemy5);

	//Entity Enemy6;
	//Enemy6.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(-1.0f, 0.0f,-1.0f), false, false);
	//AddEntity(Enemy6);

	//Entity Enemy7;
	//Enemy7.Initialise(ENEMY, CUBE, 36, m_Camera, vec3(-1.0f, 0.0f, 1.0f), false, false);
	//AddEntity(Enemy7);

	// Dot - small quad for bullet
	/*Entity Bullet;
	Bullet.Initialise(BULLET, DOT, 6, m_Camera, vec3(1.0f, 0.0f, 1.0f), false, false);
	AddEntity(Bullet);*/

	//// Pyramid
	//Model ModelPyramid;
	//vec3 positionPyramid(-2.5f, 1.9f, -2.0f);
	//ModelPyramid.Initialise(PYRAMID, 18, m_Camera, positionPyramid);
	//AddModel(ModelPyramid);

	//// Octagon
	//Model ModelOctagon;
	//vec3 positionOctagon(0.0f, 0.0f, 0.0f);
	//ModelOctagon.Initialise(OCTAGON, 18, m_Camera, positionOctagon);
	//AddModel(ModelOctagon);
}

void GameScene::CreateText()
{
	/// Text
	TextLabel Title("Robotron", "freeagent.ttf");
	Title.setPosition(glm::vec2(15, 860));
	Title.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	AddText(Title);

	TextLabel Score("Score:", "freeagent.ttf");
	Score.setPosition(glm::vec2(1250, 860));
	Score.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	AddText(Score);

	// To update text:
	/*std::string Score = "100";
	e.g.  ScoreNumber->setText(test.c_str());*/
	// Put in another function - e.g. set score
}

CClock* GameScene::GetClock()
{
	return m_pClock;
}

void GameScene::SetPositions(float _fDeltaTick)
{
	for (auto it = m_entities.begin(); it != m_entities.end(); it++)
	{
		it->SetPositions(_fDeltaTick);
	}

	for (auto it = m_bullets.begin(); it != m_bullets.end(); it++)
	{
		(*it)->SetPositions(_fDeltaTick);
	}
}


void GameScene::CheckCollisions()
{	
	// CUBE collisions - enemys and players
	for (auto it = m_entities.begin(); it != m_entities.end(); it++)
	{	
		for (auto it2 = m_entities.begin(); it2 != m_entities.end(); it2++)
		{
			if ((abs((it->GetModel()->GetPosition().x) - (it2->GetModel()->GetPosition().x)) < 1.2f) &&   // within a distance
				(abs((it->GetModel()->GetPosition().z) - (it2->GetModel()->GetPosition().z)) < 1.2f) &&   // within a distance
				((it->GetModel()->GetPosition().x) != (it2->GetModel()->GetPosition().x)) &&      // but not equal - that would be the same object
				((it->GetModel()->GetPosition().z) != (it2->GetModel()->GetPosition().z)) &&
				(it->GetModel()->GetModelType() == CUBE) && // both objects CUBES - so it doesn't include the map and other entities
				(it2->GetModel()->GetModelType() == CUBE))
			{
				if (((it->GetEntityType() == PLAYER) && (it2->GetEntityType() == ENEMY)) ||
					((it->GetEntityType() == ENEMY) && (it2->GetEntityType() == PLAYER)))

				{
					// Player dies
				}
					
				else if ((it->GetEntityType() == ENEMY) && (it2->GetEntityType() == ENEMY))
				{
					it->Flee(it2->GetModel()->GetPosition());
				//	break;
				}
			
			}

		}
	}

	// BULLET and ENEMY collisions
	for (auto it = m_entities.begin(); it != m_entities.end(); it++)
	{
		for (auto it2 = m_bullets.begin(); it2 != m_bullets.end(); it2++)
		{
			if ((abs((it->GetModel()->GetPosition().x) - ((*it2)->GetModel()->GetPosition().x)) < 1.5f) &&   // within a distance
				(abs((it->GetModel()->GetPosition().z) - ((*it2)->GetModel()->GetPosition().z)) < 1.5f) &&
				(it->GetEntityType() == ENEMY))
			{
				(*it2)->GetModel()->SetBulletToBeDeleted();
			}
		}

	}
}

//void GameScene::ExecuteOneFrame()
//{
//	//float fDT = m_pClock->GetDeltaTick();
//
//	// Process(fDT);
//	// Draw();
//
//	//m_pClock->Process();
//
//	// Sleep(15);
//}

