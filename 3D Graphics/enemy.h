//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : enemy.h
// Description : enemy declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//


#pragma once


//local includes
#include "Utils.h"
#include "prefab.h"


//class prototypes
class CTime;
class CInput;
class CModel;


class CEnemy : public CPrefab
{

public:

	CEnemy(int mx, int my) 
		: CPrefab(mx, my)
		, m_bIsDead(false)
		, m_bIsAttacking(false)
		, m_fDeathCount(0.0f)
		, m_fAttackCount(0.0f)
		, m_eState(aniState::SPAWN)
		, m_pTime(0) 
		, m_v2VecV(vec2(0.0f, 0.0f))
		, m_v2VecU(vec2(1.0f, 1.0f))
	{};
	
	void UpdateShapes(CTime* _time);

	bool GetDead();
	bool GetAttacking();
	CPrefab* GetPrefab();
	aniState GetState();

	void SetGroup(vector<CEnemy*> _group);
	void SetTarget(vec2 _target);
	void SetState(aniState state);

	void Seek(vec2 _target);
	vec2 Separation();
	void Containment();

protected:
	//class pointers
	
	CTime* m_pTime;

	float m_fDeathCount, m_fAttackCount;
	bool m_bIsDead, m_bIsAttacking;
	aniState m_eState;
	vec2 m_v2VecV, m_v2VecU, m_v2Target;
	vector<CEnemy*> m_vGroup;
};