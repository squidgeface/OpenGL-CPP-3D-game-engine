//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : player.cpp
// Description : player implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "player.h"
#include "time.h"

//set this object death state
void CPlayer::SetDead(bool _isdead)
{
	m_bIsDead = _isdead;
}
//set this hit state
void CPlayer::SetHit(bool _hit)
{
	m_bIsHit = _hit;
}
//math for moving the object in an x,y  direction
void CPlayer::PlayerMovement(vec3 direction, CTime* _time)
{
	if (direction.x != 0 || direction.y != 0 || direction.z != 0)
	{
		m_pTime = _time;
		//set position with delta time
		m_v3ObjPosition += vec3(direction.x * m_pTime->GetDelta() * 50, direction.y * m_pTime->GetDelta() * 50, direction.z * m_pTime->GetDelta() * 50);
		m_m4TranslationMatrix = translate(mat4(), m_v3ObjPosition);
	}
}
//get functions
bool CPlayer::GetHit()
{
	return m_bIsHit;
}
bool CPlayer::GetDead()
{
	return m_bIsDead;
}

//return this object prefab
CPrefab * CPlayer::GetPrefab()
{
	return this;
}

bool CPlayer::CheckModelCollision(CPrefab* _object)
{

	if (this->GetObjPosition().x + 5 > _object->GetObjPosition().x - 5 && this->GetObjPosition().x - 5 < _object->GetObjPosition().x + 5 &&
		this->GetObjPosition().z + 5 > _object->GetObjPosition().z - 5 && this->GetObjPosition().z - 5 < _object->GetObjPosition().z + 5)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CPlayer::CheckObjectCollision(CPrefab* _object)
{
	if (this->GetObjPosition().x > _object->GetObjPosition().x - (_object->GetObjSize().x / 2) && this->GetObjPosition().x  < _object->GetObjPosition().x + (_object->GetObjSize().x / 2) &&
		this->GetObjPosition().z > _object->GetObjPosition().z - (_object->GetObjSize().z / 2) && this->GetObjPosition().z  < _object->GetObjPosition().z + (_object->GetObjSize().z / 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}
