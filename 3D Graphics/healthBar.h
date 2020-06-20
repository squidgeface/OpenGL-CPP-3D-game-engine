// (c) 2020 Alexander Jenkins
//
// File Name   : healthBar.h
// Description : healthBar declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once


#include "Utils.h"
#include "player.h"

class CHealthBar : public  CPlayer
{
public:

	CHealthBar(CPlayer* _player) : CPrefab(), m_fHealthBarTimer(100.0f)
		, m_fHealthTimer(0.0f)
		, m_pTime(0)
		, m_pPlayer(_player)
	{
	};
	
	void InitialiseHealth(CCamera* camera, CTime* timer, CInput* input, vec3 _scale, vec3 _rotate, vec3 _translate);
	void LoseHealth();

private:
	
	CTime* m_pTime;
	CPlayer* m_pPlayer;
	float m_fHealthBarTimer, m_fHealthTimer;
};


