// (c) 2020 Alexander Jenkins
//
// File Name   : particle.h
// Description : particle declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "prefab.h"

class CParticle : public CPrefab
{
public:

	CParticle(int mx, int my) : CPrefab(mx, my), m_bDraw(true) {};

	void RenderShapes(GLuint program);
	void SetDraw(bool _draw);
	bool GetDraw();

protected:
	bool m_bDraw;
};