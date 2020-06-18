//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : time.h
// Description : Time declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#if !defined(__TIME_H__)
#define __TIME_H__

#include "Utils.h"

class CTime
{

public:

	CTime();
	~CTime();

	void InitiateTime();

	void UpdateTime();

	float GetDelta();

	float GetTime();

private:
	CTime(const CTime& _kr);
	
protected:
	//variables
	double m_fTimeElapsed, m_fDeltaTime, m_fLastTime, m_fCurrentTime, m_fSecondsPerCount;
	int m_iFrameCount = 0;
};

#endif