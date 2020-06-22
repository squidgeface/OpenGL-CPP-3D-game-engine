// (c) 2020 Alexander Jenkins
//
// File Name   : bullet.h
// Description : bullet declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "player.h"

class CBullet : public CPlayer
{
public:

	CBullet(int mx, int my) : CPrefab(mx, my), m_bDraw(true) {};

	//functions
	void RenderShapes(GLuint program, int _slot = 1) override;
	void SetDraw(bool _draw);
	bool GetDraw();

protected:

	bool m_bDraw;

};

