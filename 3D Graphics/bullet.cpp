//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : bullet.h
// Description : bullet implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "bullet.h"

//render
void CBullet::RenderShapes(GLuint program, int _slot)
{
	if (m_bDraw)
	{
		CPrefab::RenderShapes(program);
	}
}
//set draw
void CBullet::SetDraw(bool _draw)
{
	m_bDraw = _draw;
}
//return draw bool
bool CBullet::GetDraw()
{
	return m_bDraw;
}
