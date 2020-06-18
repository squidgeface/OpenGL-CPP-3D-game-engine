//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : Utils.h
// Description : Utils declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//
#pragma once

#if !defined(__UTILS_H__)
#define __UTILS_H__

// Dependency Includes
#include <glew.h>
#include <freeglut.h>
#include <SOIL.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <fmod.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

//Library includes
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <math.h>
#include <fstream>
#include <sstream>

//Local includes
#include "ShaderLoader.h"

constexpr auto PI = 3.14159265359;

//namespace includes
using namespace std;
using namespace glm;
using namespace FMOD;

//Input states
enum class InputState
{
	INPUT_UP,
	INPUT_DOWN,
	INPUT_UP_FIRST,
	INPUT_DOWN_FIRST,
};

enum class MeshType
{
	ANIQUAD,
	QUAD,
	PYRAMID,
	CUBE,
	BLANK,
	SPHERE,
	MODEL,
};

enum class aniState {
	NONE,
	SPAWN,
	ATTACK,
	DEATH,
};

//namespace define
namespace Utils
{
	const int ScreenWidth = 1280;
	const int ScreenHeight = 768;
	const float HalfScreenW = ScreenWidth / 2;
	const float HalfScreenH = ScreenHeight / 2;
}

#endif    // __UTILS_H__