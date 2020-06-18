//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : player.h
// Description : player header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Utils.h"
#include "prefab.h"

class CHealthBar;
class CTime;



class CPlayer : public virtual CPrefab
{

public:

	CPlayer() : CPrefab(), m_bIsHit(false), m_bOuch(false), m_bIsDead(false), m_pTime(0){};
	
	void SetDead(bool _isdead);
	bool GetHit();
	void SetHit(bool _hit);
	bool GetDead();
	void PlayerMovement(vec3 direction, CTime* _time);
	CPrefab* GetPrefab();
	bool CheckModelCollision(CPrefab* _object);
	bool CheckObjectCollision(CPrefab* _object);

protected:

	CTime* m_pTime;
	bool m_bIsDead, m_bIsHit, m_bOuch;
};

#endif //__PLAYER_H__