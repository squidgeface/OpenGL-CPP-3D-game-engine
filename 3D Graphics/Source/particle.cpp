// (c) 2020 Alexander Jenkins
//
// File Name   : particle.cpp
// Description : particle implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "particle.h"

//render
void CParticle::RenderShapes(GLuint program)
{
	if (m_bDraw)
	{
		CPrefab::RenderShapes(program);
	}
}
//set draw bool
void CParticle::SetDraw(bool _draw)
{
	m_bDraw = _draw;
}
//get draw bool
bool CParticle::GetDraw()
{
	return m_bDraw;
}