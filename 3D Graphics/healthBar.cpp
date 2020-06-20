// (c) 2020 Alexander Jenkins
//
// File Name   : healthBar.cpp
// Description : healthBar implementaion file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "healthBar.h"

void CHealthBar::InitialiseHealth(CCamera* camera, CTime* timer, CInput* input, vec3 _scale, vec3 _rotate, vec3 _translate)
{
	CPrefab::Initialise(camera, timer, input, MeshType::QUAD, "", 0, vec3(550, 28, 1.0), vec3(), vec3(0.0f, -305.0f, 0.0f));
	m_pTime = timer;
	CPrefab::InitialiseTextures("Resources/Textures/healthBar.png", 1);
}

void CHealthBar::LoseHealth()
{
	//player gets hit by an enemy
	if (m_pPlayer->GetHit() && !m_pPlayer->GetDead())
	{
		//set ouch to true and health bar shrink
		m_pPlayer->SetHit(false);
		this->Shrink(50.0f, 0.0f, 0.0f);
	}
	//set dead when healthbar = 0
	if (this->GetObjSize().x <= 0)
	{
		m_pPlayer->SetDead(true);
	}
}
