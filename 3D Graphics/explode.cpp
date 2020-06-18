//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : explodeh
// Description : explode implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "explode.h"
#include "particle.h"

CExplode::CExplode()
	: m_bIsOn(false)
{
}

CExplode::~CExplode()
{
	//clear vectors
	while (m_vParticles.size() > 0)
	{
		CParticle* aParticle = m_vParticles[m_vParticles.size() - 1];
		m_vParticles.pop_back();
		delete aParticle;
	}
}

//initialise sprite meshes type, frame count, scalse, rotation and position
void CExplode::Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount, vec3 _scale, vec3 _rotate, vec3 _translate)
{
	//the first 1/3 sprites with variable sizes
	for (unsigned int i = 0; i < frameCount / 3.0f; i++)
	{
		srand((unsigned int)(rand() ^ time(NULL)));
		float xrand = static_cast<float>(rand() % 3 + 3);
		float yrand = static_cast<float>(rand() % 3 + 3);
		float zrand = static_cast<float>(rand() % 3 + 3);
		CParticle* particle = new CParticle((int)_translate.x, (int)_translate.y);
		particle->Initialise(camera, timer, input, type, "", frameCount, vec3(xrand, yrand, zrand), _rotate, _translate);
		m_vParticles.push_back(particle);
	}
	for (unsigned int i = 0; i < frameCount / 3.0f; i++)
	{
		srand((unsigned int)(rand() ^ time(NULL)));
		float xrand = static_cast<float>(rand() % 5 + 5);
		float yrand = static_cast<float>(rand() % 5 + 3);
		float zrand = static_cast<float>(rand() % 1 + 1);
		CParticle* particle = new CParticle((int)_translate.x, (int)_translate.y);
		particle->Initialise(camera, timer, input, type, "", frameCount, vec3(xrand, yrand, zrand), _rotate, _translate);
		m_vParticles.push_back(particle);
	}
	for (unsigned int i = 0; i < frameCount / 3.0f; i++)
	{
		srand((unsigned int)(rand() ^ time(NULL)));
		float xrand = static_cast<float>(rand() % 5 + 1);
		float yrand = static_cast<float>(rand() % 5 + 1);
		float zrand = static_cast<float>(rand() % 1 + 3);
		CParticle* particle = new CParticle((int)_translate.x, (int)_translate.y);
		particle->Initialise(camera, timer, input, type, "", frameCount, vec3(xrand, yrand, zrand), _rotate, _translate);
		m_vParticles.push_back(particle);
	}
}
//initialise particle textures with 1/3 particle 1, 1/3 particle 2 and 1/3 particle 3 
void CExplode::InitialiseTextures()
{
	for (size_t i = 0; i < (size_t)(m_vParticles.size()/3.0f); i++)
	{
		m_vParticles[i]->InitialiseTextures("Resources/Textures/particle.png", 1);
	}
	for (size_t i = (size_t)(m_vParticles.size() / 3.0f); i < (size_t)((m_vParticles.size()/3.0f)*2); i++)
	{
		m_vParticles[i]->InitialiseTextures("Resources/Textures/particle2.png", 1);
	}
	for (size_t i = (size_t)((m_vParticles.size() / 3.0f) * 2); i < (size_t)((m_vParticles.size() / 3.0f) * 3); i++)
	{
		m_vParticles[i]->InitialiseTextures("Resources/Textures/particle3.png", 1);
	}
}
//render particles
void CExplode::RenderShapes(GLuint program)
{
	for (size_t i = 0; i < m_vParticles.size(); i++)
	{
		m_vParticles[i]->RenderShapes(program);
	}
}
//update particles
void CExplode::UpdateShapes()
{
	for (size_t i = 0; i < m_vParticles.size(); i++)
	{
		m_vParticles[i]->UpdateShapes();
		if (m_vParticles[i]->GetObjSize().x < 0 || m_vParticles[i]->GetObjSize().z < 0)
		{
			CParticle* aParticle = m_vParticles[i];
			m_vParticles.erase(m_vParticles.begin() + i);
			delete aParticle;
		}
	}
	//if explotions is active
	if (m_bIsOn)
	{
		SetExplode();
	}
	
}
//explode the particles
void CExplode::SetExplode()
{
	for (size_t i = 0; i < m_vParticles.size(); i++)
	{
		//if the particle x size is > 0
		if (m_vParticles[i]->GetObjSize().x > 0 || m_vParticles[i]->GetObjSize().y > 0 || m_vParticles[i]->GetObjSize().z > 0)
		{
			//random position variables for each particle
			srand((unsigned int)(rand() ^ time(NULL)));
			float zrand = rand() % 200 - 100.0f;
			float xrand = rand() % 165 - 95.0f;
			m_vParticles[i]->SetObjPosition(xrand, 0.0f, zrand);
			m_vParticles[i]->SetDraw(true);
			//shrink particles over time
			m_vParticles[i]->Shrink(0.1f,0.1f,0.1f);
		}
	}
	//if particles vector size == 0 stop drawing
	if (m_vParticles.size() == 0)
	{
		m_bIsOn = false;
	}
}
//set activated bool
void CExplode::SetActive(bool _isOn)
{
	m_bIsOn = _isOn;
}
//get activated bool
bool CExplode::GetActive()
{
	return m_bIsOn;
}

