// (c) 2020 Alexander Jenkins
//
// File Name   : explode
// Description : explode declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "prefab.h"

class CParticle;

class CExplode : public CPrefab
{

public:

	CExplode();
	~CExplode();

	void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount = 0.0f, vec3 scale = vec3(0.0, 0.0, 0.0), vec3 rotate = vec3(0.0, 0.0, 0.0), vec3 translate = vec3(0.0, 0.0, 0.0));
	void InitialiseTextures();
	void RenderShapes(GLuint program);
	void UpdateShapes();
	void SetExplode();
	void SetActive(bool _isOn);
	bool GetActive();

protected:
	//Variables
	vector<CParticle*> m_vParticles;
	bool m_bIsOn;

};