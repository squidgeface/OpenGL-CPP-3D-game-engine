//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : enemy.cpp
// Description : enemy implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "enemy.h"
#include "time.h"

//set animation state
void CEnemy::SetState(aniState aState)
{
	m_eState = aState;
}

//get animation state
aniState CEnemy::GetState()
{
	return m_eState;
}

//Update shape and time variables
void CEnemy::UpdateShapes(CTime* _time)
{
	CPrefab::UpdateShapes();
	m_pTime = _time;
	
	switch (m_eState)
	{
		case aniState::SPAWN:
		{
			//Spawn animation
			SetObjPosition(m_v3ObjPosition.x, m_v3ObjPosition.y - m_pTime->GetDelta() * 50, m_v3ObjPosition.z);

			//wait for spawn animation to switch to attack
			if (m_fAttackCount > 80.0f)
			{
				//start attacking
				m_bIsAttacking = true;
			}
			m_fAttackCount += m_pTime->GetDelta() * 100;

			break;
		}
		case aniState::ATTACK:
		{
			Seek(m_v2Target);
			m_bIsAttacking = false;

			break;
		}
		case aniState::DEATH:
		{
			//Death animation
			vec2 DeathPos;
			DeathPos.x = m_v3ObjPosition.z - m_v2Target.x;
			DeathPos.y = m_v3ObjPosition.x - m_v2Target.y;
			DeathPos = normalize(DeathPos);
			SetObjPosition(m_v3ObjPosition.x + DeathPos.y * m_pTime->GetDelta() * 50, m_v3ObjPosition.y, m_v3ObjPosition.z + DeathPos.x * m_pTime->GetDelta() * 50);
			m_fRotationAngle = atan2(DeathPos.x, DeathPos.y);
			m_v3RotationAxisY = vec3(1.0f, 0.0f, 1.0f);
			m_m4RotationZ += rotate(mat4(), m_fRotationAngle, m_v3RotationAxisY);

			//wait for death animation
			if (m_fDeathCount > 25.0f)
			{
				m_bIsDead = true;
			}
			m_fDeathCount += m_pTime->GetDelta() * 100;

			break;
		}
		default:
		{
			break;
		}
	}

	//contain enemies in the bounds of the level
	Containment();
}

//return death bool
bool CEnemy::GetDead()
{
	return m_bIsDead;
}
//return attacking bool
bool CEnemy::GetAttacking()
{
	return m_bIsAttacking;
}

CPrefab* CEnemy::GetPrefab()
{
	return this;
}

void CEnemy::SetGroup(vector<CEnemy*> _group)
{
	m_vGroup = _group;
}

void CEnemy::SetTarget(vec2 _target)
{
	m_v2Target = _target;
}

//math for seeking out the target
void CEnemy::Seek(vec2 _target)
{
	//accelleration vector
	vec2 vecA = vec2(0.0f, 0.0f);
	float m_fMaxSpeed = 0.5f;
	float m_fMaxForce = 0.1f;

	//target - object vector
	vec2 m_v2VecV;
	m_v2VecV.x = _target.x - m_v3ObjPosition.x;
	m_v2VecV.y = _target.y - m_v3ObjPosition.z;

	//normalise to max speed
	m_v2VecV = normalize(m_v2VecV) * m_fMaxSpeed;
	//force vector
	vec2 vecF = m_v2VecV - m_v2VecU;
	//limit force vector
	if (Magnitude(vecF) > m_fMaxForce)
	{
		vecF = normalize(vecF) * m_fMaxForce;
	}
	//add force to accelleration vector
	vecA += vecF;

	//add accelleration vector to initial object vector
	m_v2VecU += vecA;

	//limit object vector
	if (Magnitude(m_v2VecU) > m_fMaxSpeed)
	{
		m_v2VecU = normalize(m_v2VecU) * m_fMaxSpeed;
	}

	//apply rotations for turning along trajectory
	m_fRotationAngle = atan2f(-m_v2VecU.y, -m_v2VecU.x);
	m_v3RotationAxisZ = vec3(0.0f, 1.0f, 0.0f);
	m_m4RotationZ = rotate(mat4(), -m_fRotationAngle, m_v3RotationAxisZ);

	//keeps enemies separated from each other
	m_v2VecU += Separation();

	//limit object vector
	if (Magnitude(m_v2VecU) > m_fMaxSpeed)
	{
		m_v2VecU = normalize(m_v2VecU) * m_fMaxSpeed;
	}

	float enemySpeed = 80.0f;

	//apply position to object
	m_v3ObjPosition += vec3(m_v2VecU.x * m_pTime->GetDelta() * enemySpeed, 0.0f, m_v2VecU.y * m_pTime->GetDelta() * enemySpeed);
	m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);
}

//keeps enemies separate from each other
vec2 CEnemy::Separation()
{
	//separation
	vec2 vecSep;
	int neighbourCount = 0;
	//check entire incoming group
	for (size_t i = 0; i < m_vGroup.size(); i++)
	{
		//if this object is not being checked against itself
		if (this != m_vGroup[i]->GetPrefab())
		{
			//if the distance of this object with its neighbours is less than this object size
			if (Distance(vec2(m_v3ObjPosition.x, m_v3ObjPosition.z), vec2(m_vGroup[i]->GetObjPosition().x, m_vGroup[i]->GetObjPosition().z)) < 10)
			{
				//add all this to neghbour vectors
				vecSep.x += m_vGroup[i]->GetObjPosition().x - m_v3ObjPosition.x * m_pTime->GetDelta() * 100;
				vecSep.y += m_vGroup[i]->GetObjPosition().z - m_v3ObjPosition.z * m_pTime->GetDelta() * 100;
				//count up all the neightbours
				neighbourCount++;
			}
		}
	}
	if (neighbourCount != 0)
	{
		//get average vector to move to
		vecSep.x /= neighbourCount;
		vecSep.y /= neighbourCount;
		//reverse the vector to move away
		vecSep.x *= -1;
		vecSep.y *= -1;
		//normalize this and times by 
		vecSep = normalize(vecSep);
		return vecSep;
	}
	else
	{
		return vec2(0.0f, 0.0f);
	}

}
//keeps enemies confined withing the level boundaries
void CEnemy::Containment()
{
	//set up boundary collision for movement
		//Left wall collision
	if (GetObjPosition().z <= -95)
	{
		SetObjPosition(GetObjPosition().x, 0.0f, -95);
		m_v2VecU.y *= -1;
	}

	//Right wall collision
	if (GetObjPosition().z >= 95)
	{
		SetObjPosition(GetObjPosition().x, 0.0f, 95);
		m_v2VecU.y *= -1;
	}

	//Top wall collision
	if (GetObjPosition().x <= -85)
	{
		SetObjPosition(-85, 0.0f, GetObjPosition().z);
		m_v2VecU.x *= -1;
	}

	//Bottom wall collision
	if (GetObjPosition().x >= 70)
	{
		SetObjPosition(70, 0.0f, GetObjPosition().z);
		m_v2VecU.x *= -1;
	}

}