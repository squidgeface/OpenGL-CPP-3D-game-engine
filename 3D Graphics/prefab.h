//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : prefab.h
// Description : prefab header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//


#pragma once

#include "Utils.h"


class CMesh;
class CTexture;
class CCamera;
class CTime;
class CInput;
class CCubemap;
class CModel;


class CPrefab
{

public:

	CPrefab();
	CPrefab(int mx, int my);
	~CPrefab();

	//Game Functions
	virtual void Initialise(CCamera* camera, CTime* timer, CInput* input, MeshType type, string path, float frameCount = 0.0f, vec3 scale = vec3(0.0, 0.0, 0.0), vec3 rotate = vec3(0.0, 0.0, 0.0), vec3 translate = vec3(0.0, 0.0, 0.0));
	void InitialiseTextures(const char* data, int slot);
	virtual void RenderShapes(GLuint program, int slot = 1);
	virtual void UpdateShapes(CCubemap* cubeMap = NULL, CPrefab* _Object = NULL);

	//math functions
	float Magnitude(vec2 _source);
	float Distance(vec2 _source, vec2 _target);

	//Object manipulation functions
	void Shrink(float xScale, float yScale, float zScale);
	void RotateToObject(CPrefab* _object);

	//set functions
	void SetLightPosition(vec3 _lightPos);
	void SetLightColour(vec3 _lightCol);
	void SetModel(CModel* _model);
	void SetRotating();

	void SetObjPosition(float x, float y, float z);
	void SetObjectVector();


	//get functions
	vec3 GetObjSize();
	vec3 GetObjPosition();

protected:

	//class pointers
	CMesh* m_pMesh;
	CTexture* m_pTexture;
	CCamera* m_pCamera;
	CTime* m_pTime;
	CInput* m_pInput;
	CCubemap* m_pCubeMap;
	CModel* m_pObjMesh;

	//Matrix manipulation variables
	vec3 m_v3ObjPosition, m_v3RotationAxisX, m_v3RotationAxisY, m_v3RotationAxisZ, m_v3ObjScale;
	mat4 m_m4TranslationMatrix, m_m4RotationX, m_m4RotationZ, m_m4RotationY, m_m4ScaleMatrix, model, MVP;
	float m_fRotationAngle, m_fTexPos;
	bool m_bRotations;
	const int m_iMx, m_iMy;
	int m_iX, m_iY;
	MeshType m_eMeshType;
	vec3 m_v3LightPosition;
	vec3 m_v3LightColour;
	
};