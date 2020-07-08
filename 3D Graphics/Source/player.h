// (c) 2020 Alexander Jenkins
//
// File Name   : player.h
// Description : player declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"
#include "prefab.h"

class CHealthBar;
class CTime;
class CCamera;

class CPlayer : public virtual CPrefab
{

public:

	CPlayer() : CPrefab(), m_bIsHit(false), m_bOuch(false), m_bIsDead(false), m_pTime(0){};
	
	bool GetDead();
	void SetDead(bool _isdead);

	bool GetHit();
	void SetHit(bool _hit);

	void PlayerMovement(vec3 direction, CTime* _time);
	void MoveCamera(CCamera* _camera);
	bool CheckModelCollision(CPrefab* _object);
	bool CheckObjectCollision(CPrefab* _object);



protected:

	CTime* m_pTime;
	bool m_bIsDead, m_bIsHit, m_bOuch;
};