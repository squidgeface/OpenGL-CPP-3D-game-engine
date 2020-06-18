//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : gameManager.h
// Description : gameManager header file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once
#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

//local includes
#include "Utils.h"

//class prototypes
class CPrefab;
class CPlayer;
class CInput;
class CTime;
class CCamera;
class CTextLabel;
class CEnemy;
class CBullet;
class CExplode;
class CAudio;
class CHealthBar;
class CCubemap;
class CButton;
class CModel;

//enum class
enum class GameState
{
	MENU,
	GAME,
	LOSE,
};

class CGameManager
{
public:

	CGameManager();
	~CGameManager();
	//functions
	void InitialiseWindow(int argc, char **argv);
	void InitialiseMenu();
	void ClearMenu();
	void InitialiseGame();
	void ClearGame();
	void InitialiseGameover();
	void ClearGameover();
	void InitialiseSoundSystem();
	void InitialiseAudioFiles();
	void PlaySounds(Sound* holder, bool bLoop = false);
	void KeyboardDown(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void MouseClick(int button, int state, int x, int y);
	void MouseMove(int x, int y);
	void Render();
	void Update();
	void PlayerRun();
	void EnemySpawner();
	void BulletEnemyCollision();
	void PlayerEnemyCollision();
	void SpawnPowerUp();
	void PowerUpRand();
	void PlayerPowerupCollision();
	void EnemyDeathCheck();
	void ProcessInput(InputState* KeyState, InputState* MouseState);

	void Initialise2DPointer();

	void Initialise3DPointer();

protected:
	//Variables
	//sound FX
		//sound FX
	Sound* m_pFxBullet;
	Sound* m_pFxScream;
	Sound* m_pFxSplat;
	Sound* m_pFxHit;
	Sound* m_pFxBtn;
	Sound* m_pFxPlay;
	Sound* m_pTrackBackground;
	Sound* m_pFxExplode;

	//shader programs
	GLuint m_giStaticProgram, m_giPlayerProgram, m_giTextProgram;
	GLuint m_giPhongProgram;
	GLuint m_giBlinnProgram;
	GLuint m_giRimProgram;
	GLuint m_giCubeMapProgram;
	GLuint m_giReflectCubeMapProgram;

	CCubemap* m_giCubeMap;

	//Game objects
	CAudio* m_pAudioSystem;
	CExplode* m_pExplosion;
	CPlayer* m_pPlayerPrefab;
	CPrefab* m_pArena;
	CPrefab* m_pWall;
	CPrefab* m_pPointer;
	CPrefab* m_pPowerUp;
	CPrefab* m_pGameBackground;
	CPrefab* m_pMenuBackground;
	CPrefab* m_pWalls;
	CPrefab* m_pScore;

	CButton* m_pPlayBtn;
	CButton* m_pCreditsBtn;
	CButton* m_pQuitBtn;
	CButton* m_pRetryBtn;
	CButton* m_pCreditsLayer;
	CPrefab* m_pHidden;

	CPrefab* m_pDiscoBallUL;
	CPrefab* m_pDiscoBallUR;
	CPrefab* m_pDiscoBallLL;
	CPrefab* m_pDiscoBallLR;
	CHealthBar* m_pHealthBar;
	CModel* m_pEnemyModel;

	//Game Object vectors
	vector<CBullet*> m_vBullets;
	vector<CEnemy*> m_vEnemySpriteVec;
	vector<CPrefab*> m_vArenaWallVec;
	vector<CPrefab*> m_vDiscoVec;

	//Game management objects
	CInput* m_pInput;
	CTime* m_pTime;
	CCamera* m_pOrthoCamera;
	CCamera* m_pProjCamera;

	//text labels
	CTextLabel* scoreText;
	CTextLabel* hiscoreText;

	//counters and rates
	float m_fcounter, m_fspawnCount, m_fplayerSpeed, m_fspawnRate, m_fHitTimer, m_fhorizontal, m_fvertical;
	int m_ipUpCount, m_iscore, m_ihiscore, m_ihover, m_ispawnSize;
	//bools and switches
	bool m_bgoingL, m_bgoingR, m_bgoingU, m_bgoingD, m_blose, m_bhiScores, m_bspawnPup, m_bGameStart, m_bPaused;
	//enum
	GameState m_eGameState;
};

#endif //__GAMEMANAGER_H__