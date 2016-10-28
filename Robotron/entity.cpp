//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2016 Media Design School
//
// File Name : Entity.cpp
// Description : Entity implementation file 
// Mail : Mitchell.Currie@mediadesignschool.com
//

#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>
#include <time.h>  

#include "glew.h"
#include "freeglut.h"
#include "entity.h"
#include "ShaderLoader.h"
#include "utils.h"

unsigned char keyState[255];

vec3 Entity::m_CurrentPlayerVelocity = vec3(0.0f, 0.0f, 0.0f);
vec3 Entity::m_CurrentLeaderVelocity = vec3(0.0f, 0.0f, 0.0f);
bool Entity::m_bBulletFired = false;
std::clock_t Entity::m_start = std::clock();
double Entity::m_duration = 0.0f;
std::string Entity::m_AIName = "";
vec2 Entity::m_textPosition = vec2(0.0f, 0.0f);
bool Entity::m_bLeaderDead = false;

Entity::Entity()
{
	m_CurrentVelocity = vec3(0.0f, 0.0f, 0.0f);

	// To update:
	m_fMaxForce = 0.05f;
	m_fMaxVelocity = 0.05f;
	SetBulletDirection = false;
	srand(time(NULL));
}

Entity::~Entity()
{

}

void Entity::Initialise(EntityType _entity, ModelType _model, GLsizei _numVertices, Camera _camera, vec3 _position, bool _IsPlayer, bool _IsLeader)
{
	m_pModel = new Model;
	m_pModel->Initialise(_model, _numVertices, _camera, _position, _IsPlayer, _IsLeader);
	m_EntityType = _entity;

	if (m_EntityType == PLAYER)
		m_start = std::clock();
}


void Entity::Render()
{
	m_pModel->Render(m_CurrentVelocity);
}

void Entity::SetPositions(float _fDeltaTick)
{
	if (m_EntityType == PLAYER)
	{
		m_bBulletFired = false;

		float fSpeed = 8;

		// Forward left Movement
		if ((keyState[(unsigned char)'a'] == BUTTON_DOWN) && (keyState[(unsigned char)'w'] == BUTTON_DOWN))
		{
			printf("Move cube forward left \n");
			m_CurrentVelocity = glm::vec3(-fSpeed, 0.0f, -fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Forward right Movement
		else if ((keyState[(unsigned char)'d'] == BUTTON_DOWN) && (keyState[(unsigned char)'w'] == BUTTON_DOWN))
		{
			printf("Move cube forward right \n");
			m_CurrentVelocity = glm::vec3(fSpeed, 0.0f, -fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Backward left Movement
		else if ((keyState[(unsigned char)'a'] == BUTTON_DOWN) && (keyState[(unsigned char)'s'] == BUTTON_DOWN))
		{
			printf("Move cube backward left \n");
			m_CurrentVelocity = glm::vec3(-fSpeed, 0.0f, fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Backward right Movement
		else if ((keyState[(unsigned char)'d'] == BUTTON_DOWN) && (keyState[(unsigned char)'s'] == BUTTON_DOWN))
		{
			printf("Move cube backward right \n");
			m_CurrentVelocity = glm::vec3(fSpeed, 0.0f, fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Left Movement
		else if (keyState[(unsigned char)'a'] == BUTTON_DOWN)
		{
			printf("Move cube left \n");
			m_CurrentVelocity = glm::vec3(-fSpeed, 0.0f, 0.0f) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Right movement
		else if (keyState[(unsigned char)'d'] == BUTTON_DOWN)
		{
			printf("Move cube right \n");
			m_CurrentVelocity = glm::vec3(fSpeed, 0.0f, 0.0f) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Forward movement
		else if (keyState[(unsigned char)'w'] == BUTTON_DOWN)
		{
			printf("Move cube forward \n");
			m_CurrentVelocity = glm::vec3(0.0f, 0.0f, -fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		// Back movement
		else if (keyState[(unsigned char)'s'] == BUTTON_DOWN)
		{
			printf("Move cube back \n");
			m_CurrentVelocity = glm::vec3(0.0f, 0.0f, fSpeed) * _fDeltaTick;
			m_pModel->SetLastPlayerVelocity(m_CurrentVelocity);
		}

		else
		{
			m_CurrentVelocity = vec3(0.0f, 0.0f, 0.0f);
		}

		if (keyState[(unsigned char)'l'] == BUTTON_DOWN)
		{
			m_duration = (std::clock() - m_start) / (double)CLOCKS_PER_SEC;
			
			if (m_duration > 0.15f)
			{
				printf("Fire \n");
				m_bBulletFired = true;
				m_start = std::clock();
			}		
		}

		m_CurrentPlayerVelocity = m_CurrentVelocity;
	}

	else if (m_EntityType == ENEMY)// Enemy movement
	{		
		/*Seek(m_pModel->GetPlayerPosition());
		m_AIName = "Seek";
		m_textPosition = vec2(720, 860);*/

		/*Flee(m_pModel->GetPlayerPosition());
		m_AIName = "Flee";
		m_textPosition = vec2(720, 860);*/

		/*Pursue();
		m_AIName = "Pursue";
		m_textPosition = vec2(670, 860);*/ 

		/*Evade();
		m_AIName = "Evade";
		m_textPosition = vec2(690, 860);*/

		/*Wander();
		m_AIName = "Wander";
		m_textPosition = vec2(680, 860);*/

	    LeaderFollowing();
		m_AIName = "Leader Following";
		m_textPosition = vec2(495, 860);

		/*Flocking();
		m_AIName = "Flocking";
		m_textPosition = vec2(658, 860);*/

		if (m_pModel->IsLeader())
			m_CurrentLeaderVelocity = m_CurrentVelocity;
	}

	else if (m_EntityType == BULLET)
	{
		if (!SetBulletDirection)
		{
			m_CurrentVelocity = m_pModel->GetLastPlayerVelocity() * 7.0f;
			SetBulletDirection = true;
		}
			
	}
}

bool Entity::IsPlayer()
{
	if (m_EntityType == PLAYER)
		return true;

	else
		return false;
}


void Entity::entityKeyboard(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_DOWN;
}

void Entity::entityKeyboard_up(unsigned char key, int x, int y)
{
	keyState[key] = BUTTON_UP;
}

vec3 Entity::LimitForce(vec3 _steering)
{
	vec3 Steering = _steering;
	
	if (_steering.x > m_fMaxForce)
		Steering.x = m_fMaxForce;

	if (_steering.y > m_fMaxForce)
		Steering.y = m_fMaxForce;

	if (_steering.z > m_fMaxForce)
		Steering.z = m_fMaxForce;

	return Steering;
}

vec3 Entity::LimitVelocity(vec3 _velocity)
{
	vec3 Velocity = _velocity;

	if (_velocity.x > m_fMaxVelocity)
		Velocity.x = m_fMaxVelocity;

	if (_velocity.y > m_fMaxVelocity)
		Velocity.y = m_fMaxVelocity;

	if (_velocity.z > m_fMaxVelocity)
		Velocity.z = m_fMaxVelocity;

	return Velocity;
}

void Entity::Seek(vec3 _playerPosition)
{
	vec3 DesiredVelocity = _playerPosition - m_pModel->GetPosition();
	DesiredVelocity = glm::normalize(DesiredVelocity);

	DesiredVelocity *= m_fMaxVelocity;

	vec3 SteeringForce = DesiredVelocity - m_CurrentVelocity;
	SteeringForce = LimitForce(SteeringForce);

	m_CurrentVelocity += SteeringForce;
	m_CurrentVelocity = LimitVelocity(m_CurrentVelocity);
}

void Entity::Flee(vec3 _playerPosition)
{
	vec3 DesiredVelocity = m_pModel->GetPosition() - _playerPosition;
	DesiredVelocity = glm::normalize(DesiredVelocity);

	DesiredVelocity *= m_fMaxVelocity;

	vec3 SteeringForce = DesiredVelocity - m_CurrentVelocity;
	SteeringForce = LimitForce(SteeringForce);

	m_CurrentVelocity += SteeringForce;
	m_CurrentVelocity = LimitVelocity(m_CurrentVelocity);
}

void Entity::Pursue()
{
	vec3 Distance = m_pModel->GetPlayerPosition() - m_pModel->GetPosition();

	//float T = Distance.length() / (m_fMaxVelocity + GetPlayerVelocity().length());
	float T = Distance.length() / m_fMaxVelocity;
	//float T = length(Distance) / m_fMaxVelocity;
	// float T = 50.0f;

	vec3 FuturePosition = m_pModel->GetPlayerPosition() + (m_CurrentPlayerVelocity * T);

	Seek(FuturePosition);
}

void Entity::Evade()
{
	vec3 Distance = m_pModel->GetPlayerPosition() - m_pModel->GetPosition();

	float T = Distance.length() / m_fMaxVelocity;
	//float T = 50.0f;

	vec3 FuturePosition = m_pModel->GetPlayerPosition() + m_CurrentPlayerVelocity * T;

	Flee(FuturePosition);
}

void Entity::Wander()
{
	float Distance = 1500.0f;
	float Radius = 7.0f;
	vec3 TargetPosition = vec3(0.0f, 0.0f, 0.0f);

	vec3 FuturePosition = m_pModel->GetPosition() + m_CurrentVelocity * Distance;

	float AngleDegrees = (rand() % 360) + 1;
	float AngleRads = AngleDegrees * M_PI / 180;

	TargetPosition.x = FuturePosition.x + (Radius * cos(AngleRads));
	TargetPosition.z = FuturePosition.z + (Radius * sin(AngleRads));
	
	Seek(TargetPosition);

	if (m_pModel->IsAtEdge())
		ReverseCurrentVelocity();
}

void Entity::LeaderFollowing()
{
	if (m_bLeaderDead)
	{
		m_pModel->SetLeader();
		m_bLeaderDead = false;
	}
	
	if (m_pModel->IsLeader())
	{
		Seek(m_pModel->GetPlayerPosition());
	}

	else
	{
		float Distance = 75.0f;
		vec3 BackPosition = m_pModel->GetLeaderPosition() - m_CurrentLeaderVelocity * Distance;
		
		Seek(BackPosition);
	}
}

void Entity::Flocking()
{
	if (m_bLeaderDead)
	{
		m_pModel->SetLeader();
		m_bLeaderDead = false;
	}

	if (m_pModel->IsLeader())
	{
		//Wander();
		Seek(m_pModel->GetPlayerPosition());
	}

	else
	{
		if (m_pModel->IsWithinFlockingDistance())
		{
			m_CurrentVelocity = m_CurrentLeaderVelocity;
		}

		else 
		{
			Seek(m_pModel->GetLeaderPosition());
		}
	}	
}

Model* Entity::GetModel()
{
	return m_pModel;
}

void Entity::ReverseCurrentVelocity()
{
	m_CurrentVelocity *= -1.0f;
}

vec3 Entity::GetPlayerVelocity()
{
	return m_CurrentPlayerVelocity;
}

vec3 Entity::GetCurrentVelocity()
{
	return m_CurrentVelocity;
}

void Entity::ResetCurrentVelocity()
{
	m_CurrentVelocity = vec3(0.0f, 0.0f, 0.0f);
}

EntityType Entity::GetEntityType()
{
	return m_EntityType;
}

bool Entity::IsBulletFired()
{
	return m_bBulletFired;
}

bool Entity::ToDelete()
{
	return m_pModel->ToDelete();
}

std::string Entity::GetAIName()
{
	return m_AIName;
}

vec2 Entity::GetTextPosition()
{
	return m_textPosition;
}

void Entity::SetLeaderDead()
{
	m_bLeaderDead = true;
}
