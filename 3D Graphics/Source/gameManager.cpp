// (c) 2020 Alexander Jenkins
//
// File Name   : gameManager.cpp
// Description : gameManager implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "gameManager.h"

#include "prefab.h"
#include "model.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "TextLabel.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "explode.h"
#include "audio.h"
#include "cubemap.h"
#include "healthBar.h"
#include "button.h"

CGameManager::CGameManager()
	:m_fcounter(0)
	, m_bgoingL(true)
	, m_bgoingR(true)
	, m_bgoingU(true)
	, m_bgoingD(true)
	, m_blose (false)
	, m_bPaused(false)
	, m_bGameStart(false)
	, m_bhiScores(false)
	, m_bspawnPup(false)
	, m_fplayerSpeed(1.0f)
	, m_fspawnCount(0)
	, m_iscore(0)
	, m_ihiscore(0)
	, m_ihover(0)
	, m_fspawnRate(150)
	, m_ispawnSize(100)
	, m_eGameState(GameState::MENU)
	, m_fHitTimer(0)
	, m_pMenuBackground(0)
	, m_pCreditsLayer(0)
	, m_pPlayBtn(0)
	, m_pQuitBtn(0)
	, m_pCreditsBtn(0)
	, m_pPointer(0)
	, m_pWalls(0)
	, m_pScore(0)
	, m_pPlayerPrefab(0)
	, m_pGameBackground(0)
	, scoreText(0)
	, m_pRetryBtn(0)
	, hiscoreText(0)
	, m_pAudioSystem(0)
	, m_giTextProgram(0)
	, m_giPlayerProgram(0)
	, m_giStaticProgram(0)
	, m_pPowerUp(0)
	, m_ipUpCount(0)
	, m_pExplosion(0)
	, m_fhorizontal(0)
	, m_fvertical(0)
	, m_pFxBullet(0)
	, m_pFxScream(0)
	, m_pFxSplat(0)
	, m_pFxHit(0)
	, m_pFxBtn(0)
	, m_pFxPlay(0)
	, m_pTrackBackground(0)
	, m_pFxExplode(0)
	, m_pArena(0)
	, m_giBlinnProgram(0)
	, m_giCubeMap(0)
	, m_giCubeMapProgram(0)
	, m_giPhongProgram(0)
	, m_giReflectCubeMapProgram(0)
	, m_giRimProgram(0)
	, m_pHidden(0)
	, m_pDiscoBallUL(0)
	, m_pDiscoBallUR(0)
	, m_pDiscoBallLL(0)
	, m_pDiscoBallLR(0)
	, m_pEnemyModel(0)
	, m_pHealthBar(0)
{
	m_pOrthoCamera = new CCamera();
	m_pProjCamera = new CCamera();
	m_pTime = new CTime();
	m_pInput = new CInput();
	m_pAudioSystem = new CAudio();
}

CGameManager::~CGameManager()
{
	//clear vectors
	while (m_vEnemySpriteVec.size() > 0)
	{
		CEnemy* aEnemy = m_vEnemySpriteVec[m_vEnemySpriteVec.size() - 1];
		m_vEnemySpriteVec.pop_back();
		delete aEnemy;
	}
	while (m_vBullets.size() > 0)
	{
		CBullet* aBullet = m_vBullets[m_vBullets.size() - 1];
		m_vBullets.pop_back();
		delete aBullet;
	}
	while (m_vDiscoVec.size() > 0)
	{
		CPrefab* aBall = m_vDiscoVec[m_vDiscoVec.size() - 1];
		m_vDiscoVec.pop_back();
		delete aBall;
	}
	//clear pointers
	delete m_pPlayerPrefab;
	m_pPlayerPrefab = 0;
	delete m_pPointer;
	m_pPointer = 0;
	delete m_pPowerUp;
	m_pPowerUp = 0;
	delete m_pScore;
	m_pScore = 0;
	delete m_pCreditsLayer;
	m_pCreditsLayer = 0;
	delete m_pPlayBtn;
	m_pPlayBtn = 0;
	delete m_pQuitBtn;
	m_pQuitBtn = 0;
	delete m_pCreditsBtn;
	m_pCreditsBtn = 0;
	delete m_pInput;
	m_pInput = 0;
	delete m_pTime;
	m_pTime = 0;
	delete m_pGameBackground;
	m_pGameBackground = 0;
	delete m_pMenuBackground;
	m_pMenuBackground = 0;
	delete m_pWalls;
	m_pWalls = 0;
	delete m_pOrthoCamera;
	m_pOrthoCamera = 0;
	delete scoreText;
	scoreText = 0;
	delete hiscoreText;
	hiscoreText = 0;
	delete m_pAudioSystem;
	m_pAudioSystem = 0;
	delete m_pEnemyModel;
	m_pEnemyModel = 0;
	//release sounds
	m_pFxSplat->release();
	delete m_pFxSplat;
	m_pFxSplat = 0;
	m_pFxPlay->release();
	delete m_pFxPlay;
	m_pFxPlay = 0;
	m_pFxBtn->release();
	delete m_pFxBtn;
	m_pFxBtn = 0;
	m_pFxBullet->release();
	delete m_pFxBullet;
	m_pFxBullet = 0;
	m_pFxHit->release();
	delete m_pFxHit;
	m_pFxHit = 0;
	m_pFxScream->release();
	delete m_pFxScream;
	m_pFxScream = 0;
	m_pTrackBackground->release();
	delete m_pTrackBackground;
	m_pTrackBackground = 0;
}

//Initialise Glut window
void CGameManager::InitialiseWindow(int argc, char **argv)
{
	//Setup and create glut controlled window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition((int)Utils::HalfScreenW/2, 0);
	glutInitWindowSize(Utils::ScreenWidth, Utils::ScreenHeight);
	glutCreateWindow("OpenGL First Window");

	//Set up all gl function callbacks based on p hardware
	if (glewInit() != GLEW_OK)
	{
		//if glew setup failed then applicaion will not run graphics correctly
		std::cout << "Glew Initialization Failed. Aborting App." << std::endl;
		system("pause");
	}

	//Sets the clear color when calling glclear()
	glClearColor(1.0, 0.0, 0.0, 1.0);

	//Load shader variables
	m_giStaticProgram = ShaderLoader::CreateProgram("Resources/Shaders/vertexshader.txt", "Resources/Shaders/fragshader.txt");
	m_giPlayerProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/fragshader2.txt");
	m_giPhongProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/phongFshader.txt");
	m_giBlinnProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/blinnFshader.txt");
	m_giRimProgram = ShaderLoader::CreateProgram("Resources/Shaders/3Dvertexshader.txt", "Resources/Shaders/rimFshader.txt");
	m_giCubeMapProgram = ShaderLoader::CreateProgram("Resources/Shaders/CMvertexshader.txt", "Resources/Shaders/CMFshader.txt");
	m_giReflectCubeMapProgram = ShaderLoader::CreateProgram("Resources/Shaders/CMRvertexshader.txt", "Resources/Shaders/CMRFshader.txt");

	
	//set culling on
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	//hide mouse
	glutSetCursor(GLUT_CURSOR_NONE);

	//Initialise time
	m_pTime->InitiateTime();

	//Set Up Camera from class
	m_pOrthoCamera->SetOrtho();
	m_pProjCamera->SetProjection();
}

//Initialise Menu items
void CGameManager::InitialiseMenu()
{
	//play music on first run
	if (!m_bGameStart)
	{
		m_bGameStart = true;
		//Play background music on loop
		PlaySounds(m_pTrackBackground, true);
	}

//Load shapes
	m_pMenuBackground = new CPrefab();
	m_pCreditsLayer = new CButton();
	m_pPlayBtn = new CButton();
	m_pQuitBtn = new CButton();
	m_pCreditsBtn = new CButton();
	Initialise2DPointer();

	//Menu shapes
	m_pMenuBackground->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(Utils::ScreenWidth, Utils::ScreenHeight, 1.0f), vec3(0,0,0), vec3(0, 0, 0));
	m_pPlayBtn->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(200, 45, 0), vec3(), vec3(0, 0, 0));
	m_pCreditsBtn->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(300, 40, 0), vec3(), vec3(0, 75, 0));
	m_pQuitBtn->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(200, 40, 0), vec3(), vec3(0, 150, 0));
	m_pCreditsLayer->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(Utils::ScreenWidth, Utils::ScreenHeight, 0), vec3(), vec3(0, 0, 0));

//load textures 
	//menu textures
	m_pMenuBackground->InitialiseTextures("Resources/Textures/MenuBackground.png", 1);
	m_pPlayBtn->InitialiseTextures("Resources/Textures/playButton.png", 1);
	m_pPlayBtn->InitialiseTextures("Resources/Textures/playButtonH.png", 2);
	m_pCreditsBtn->InitialiseTextures("Resources/Textures/creditsButton.png", 1);
	m_pCreditsBtn->InitialiseTextures("Resources/Textures/creditsButtonH.png", 2);
	m_pQuitBtn->InitialiseTextures("Resources/Textures/quitButton.png", 1);
	m_pQuitBtn->InitialiseTextures("Resources/Textures/quitButtonH.png", 2);
	m_pCreditsLayer->InitialiseTextures("Resources/Textures/hidden.png", 1);
	m_pCreditsLayer->InitialiseTextures("Resources/Textures/creditsLayer.png", 2);

	vector<string> mapPaths;
	mapPaths.push_back("right.jpg");
	mapPaths.push_back("left.jpg");
	mapPaths.push_back("top.jpg");
	mapPaths.push_back("bottom.jpg");
	mapPaths.push_back("back.jpg");
	mapPaths.push_back("front.jpg");
	m_giCubeMap = new CCubemap(m_pProjCamera, mapPaths);

	m_giCubeMap->Generate();
}
//clear menu items
void CGameManager::ClearMenu()
{
	//clear pointers
	delete m_pMenuBackground;
	m_pMenuBackground = 0;
	delete m_pCreditsBtn;
	m_pCreditsBtn = 0;
	delete m_pCreditsLayer;
	m_pCreditsLayer = 0;
	delete m_pPointer;
	m_pPointer = 0;
}
//initialise game items
void CGameManager::InitialiseGame()
{
//Load shapes
	m_pWalls = new CPrefab();
	m_pScore = new CPrefab();
	m_pPlayerPrefab = new CPlayer();
	m_pGameBackground = new CPrefab();
	scoreText = new CTextLabel();
	m_pPowerUp = new CPrefab();
	m_pArena = new CPrefab();
	m_pExplosion = new CExplode();
	
	m_pHidden = new CPrefab();
	m_pDiscoBallUL = new CPrefab();
	m_pDiscoBallUR = new CPrefab();
	m_pDiscoBallLL = new CPrefab();
	m_pDiscoBallLR = new CPrefab();
	m_pHealthBar = new CHealthBar(m_pPlayerPrefab);
	Initialise3DPointer();
	//Set camera looking at the center
	m_pProjCamera->LookAtObject(vec3(0.0f, 0.0f, 0.0f));

	//initial enemy model load
	m_pEnemyModel = new CModel("Resources/Models/enemy/Low_poly_UFO.obj", m_pProjCamera);
	//initialise level model
	m_pArena->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::MODEL, "Resources/Models/City/City Blocks.obj", 0, vec3(0.2f, 0.2f,0.2f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -230.0f, 0.0f));

	//game shapes
	m_pPlayerPrefab->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::MODEL, "Resources/Models/Ship/SciFi_Fighter_AK5.obj", 0, vec3(0.01f, 0.01f, 0.01f), vec3(0.0f, -90.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	m_pPlayerPrefab->SetRotating();
	m_pHealthBar->InitialiseHealth(m_pOrthoCamera, m_pTime, m_pInput, vec3(), vec3(), vec3());
	m_pWalls->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(Utils::ScreenWidth+20, Utils::ScreenHeight+20, 1.0));
	
	m_pScore->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(160, 50, 0.0f), vec3(0, 0 ,0), vec3(-80, 350, 0));
	m_pPowerUp->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(3.0f, 3.0f, 3.0f), vec3(0.0f, 180.0f, 0.0f), vec3(0.0f, 10.0f, 0.0f) );
	
	m_pHidden->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(Utils::ScreenWidth + 20, Utils::ScreenHeight + 20, 1.0));
	m_pHidden->InitialiseTextures("Resources/Textures/hidden.png", 1);
	m_pPlayBtn->SetObjPosition( 0, 55, 0);
	
//load textures 
	//game textures
	m_pPlayerPrefab->InitialiseTextures("", 1);
	m_pWalls->InitialiseTextures("Resources/Textures/walls.png", 1);
	m_pScore->InitialiseTextures("Resources/Textures/score.png", 1);
	m_pPowerUp->InitialiseTextures("Resources/Textures/cherryBomb.png", 1);

	//Initalise disco balls
	m_pDiscoBallUL->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 180.0f, 0.0f), vec3(-100.0f, 10.0f, 100.0f));
	m_vDiscoVec.push_back(m_pDiscoBallUL);
	m_pDiscoBallUR->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 180.0f, 0.0f), vec3(-100.0f, 10.0f, -100.0f));
	m_vDiscoVec.push_back(m_pDiscoBallUR);
	m_pDiscoBallLL->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 180.0f, 0.0f), vec3(70.0f, 10.0f, 100.0f));
	m_vDiscoVec.push_back(m_pDiscoBallLL);
	m_pDiscoBallLR->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(10.0f, 10.0f, 10.0f), vec3(0.0f, 180.0f, 0.0f), vec3(70.0f, 10.0f, -100.0f));
	m_vDiscoVec.push_back(m_pDiscoBallLR);
	
	for (size_t i = 0; i < m_vDiscoVec.size(); i++)
	{
		m_vDiscoVec[i]->SetRotating();
		m_vDiscoVec[i]->InitialiseTextures("Resources/Textures/discoBall.jpg", 1);
		m_vDiscoVec[i]->SetLightPosition(vec3(m_vDiscoVec[i]->GetObjPosition().x, m_vDiscoVec[i]->GetObjPosition().y + 100.0, m_vDiscoVec[i]->GetObjPosition().z));
		
	}

	//load score text
	scoreText->SetLabel(to_string(m_iscore), "Resources/Fonts/BRLNSR.ttf", vec2(10.0f, -368.0f), vec3(100.0f, 10.0f, 255.0f), 1.0f);
}

//clear game items
void CGameManager::ClearGame()
{
	//clear pointers
	delete m_pGameBackground;
	m_pGameBackground = 0;
	delete m_pPlayerPrefab;
	m_pPlayerPrefab = 0;
	delete m_pWalls;
	m_pWalls = 0;
	delete m_pScore;
	m_pScore = 0;	
	delete m_pPowerUp;
	m_pPowerUp = 0;
	delete m_pPointer;
	m_pPointer = 0;
	//clear vectors
	while (m_vEnemySpriteVec.size() > 0)
	{
		CEnemy* aEnemy = m_vEnemySpriteVec[m_vEnemySpriteVec.size() - 1];
		m_vEnemySpriteVec.pop_back();
		delete aEnemy;
	}
	while (m_vBullets.size() > 0)
	{
		CBullet* aBullet = m_vBullets[m_vBullets.size() - 1];
		m_vBullets.pop_back();
		delete aBullet;
	}
	while (m_vDiscoVec.size() > 0)
	{
		CPrefab* aBall = m_vDiscoVec[m_vDiscoVec.size() - 1];
		m_vDiscoVec.pop_back();
		delete aBall;
	}
	//reset variables
	m_fspawnRate = 150;
	m_ipUpCount = 0;
	m_bspawnPup = false;
	m_fHitTimer = 0;
}
//initialise game over items
void CGameManager::InitialiseGameover()
{
//Load shapes
	m_pQuitBtn = new CButton();
	m_pMenuBackground = new CPrefab();
	m_pRetryBtn = new CButton();
	hiscoreText = new CTextLabel();
	hiscoreText->SetLabel("Hi Score: " + to_string(m_ihiscore), "Resources/Fonts/BRLNSR.ttf", vec2(200.0f, 360.0f), vec3(100.0f, 200.0f, 255.0f), 0.5f);
	m_pPointer = new CPrefab();

	//Menu shapes
	m_pMenuBackground->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3( Utils::ScreenWidth + 20, Utils::ScreenHeight + 20, 0), vec3());
	m_pQuitBtn->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(200, 40, 0), vec3(), vec3(0, 150, 0));
	m_pRetryBtn->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(200, 40, 0), vec3(), vec3(0, 100, 0));

	//2D pointer
	m_pPointer->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0, 40.0, 0));
	m_pPointer->InitialiseTextures("Resources/Textures/pointer.png", 1);

//load textures 
	//menu textures
	m_pMenuBackground->InitialiseTextures("Resources/Textures/loseBackground.png", 1);
	m_pQuitBtn->InitialiseTextures("Resources/Textures/quitButton.png", 1);
	m_pQuitBtn->InitialiseTextures("Resources/Textures/quitButtonH.png", 2);
	m_pRetryBtn->InitialiseTextures("Resources/Textures/retryButton.png", 1);
	m_pRetryBtn->InitialiseTextures("Resources/Textures/retryButtonH.png", 2);
}
//clear game over items
void CGameManager::ClearGameover()
{
	//clear pointers
	delete m_pMenuBackground;
	m_pMenuBackground = 0;
	
	delete m_pRetryBtn;
	m_pRetryBtn = 0;
	delete m_pPointer;
	m_pPointer = 0;
}

//initialise fmod sound system
void CGameManager::InitialiseSoundSystem()
{
	m_pAudioSystem->InitialiseSoundSystem();
}
//initialise audio files
void CGameManager::InitialiseAudioFiles()
{
	
		FMOD_RESULT result;
		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/Background.mp3",
			FMOD_DEFAULT,
			0,
			&m_pTrackBackground
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/Thump.wav",
			FMOD_DEFAULT,
			0,
			&m_pFxBullet
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/splat.wav",
			FMOD_DEFAULT,
			0,
			&m_pFxSplat
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/scream.mp3",
			FMOD_DEFAULT,
			0,
			&m_pFxScream
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/ouch.mp3",
			FMOD_DEFAULT,
			0,
			&m_pFxHit
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/play.mp3",
			FMOD_DEFAULT,
			0,
			&m_pFxPlay
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/buttonH.mp3",
			FMOD_DEFAULT,
			0,
			&m_pFxBtn
		);

		result = m_pAudioSystem->GetAudioSystem()->createSound(
			"Resources/Audio/explode.mp3",
			FMOD_DEFAULT,
			0,
			&m_pFxExplode
		);

}
//play sounds function
void CGameManager::PlaySounds(Sound* holder, bool bLoop)
{
	m_pAudioSystem->PlaySounds(holder, bLoop);
}

//keyboard input functions
void CGameManager::KeyboardDown(unsigned char key, int x, int y)
{
	m_pInput->KeyboardDown(key, x, y);
}
void CGameManager::KeyboardUp(unsigned char key, int x, int y)
{
	m_pInput->KeyboardUp(key, x, y);
}

//mouse input functions
void CGameManager::MouseClick(int button, int state, int x, int y)
{
	m_pInput->MouseClick(button, state, x, y);
}
void CGameManager::MouseMove(int x, int y)
{
	m_pInput->MouseMove(x, y);
}

//render function
void CGameManager::Render()
{
	//Clear buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//switch for game state
	switch (m_eGameState)
	{
	case GameState::MENU:
	//render menu shapes
		m_giCubeMap->Render(m_giCubeMapProgram);
		if (!m_pCreditsLayer->GetShowing())
		{
			m_pMenuBackground->RenderShapes(m_giStaticProgram);
			m_pPlayBtn->RenderShapes(m_giStaticProgram);
			m_pCreditsBtn->RenderShapes(m_giStaticProgram);
			m_pQuitBtn->RenderShapes(m_giStaticProgram);
		}
		else
		{
			m_pCreditsLayer->RenderShapes(m_giStaticProgram);
		}

		break;
	case GameState::GAME:
	//Vertex and shader renders (order metters)
		m_giCubeMap->Render(m_giCubeMapProgram);
		//gameBackground->renderShapes(staticProgram);
		m_pArena->RenderShapes(m_giBlinnProgram);
		//player render
		m_pPlayerPrefab->RenderShapes(m_giBlinnProgram);
		

	//render vectors for enemies and bullets
		for (size_t i = 0; i < m_vEnemySpriteVec.size(); i++)
		{
			m_vEnemySpriteVec[i]->RenderShapes(m_giBlinnProgram);
		}
		for (size_t i = 0; i < m_vBullets.size(); i++)
		{
			m_vBullets[i]->RenderShapes(m_giBlinnProgram);
		}

	//render walls and score last
		m_pWalls->RenderShapes(m_giStaticProgram);
		m_pScore->RenderShapes(m_giStaticProgram);
		

	//UI render
		scoreText->Render();

	//if game finished once, show hi score
		if (m_bhiScores)
		{
			hiscoreText->Render();
		}
		
	//render powerup on spawn
		if (m_bspawnPup)
		{
			m_pPowerUp->RenderShapes(m_giPhongProgram);
			
		}
		//render disco balls with reflect cubemap
		for (size_t i = 0; i < m_vDiscoVec.size(); i++)
		{
			m_vDiscoVec[i]->RenderShapes(m_giReflectCubeMapProgram);
	
		}

		//UI Health
		m_pHealthBar->RenderShapes(m_giStaticProgram);

	//if explosion is true render shapes
		if (m_pExplosion->GetActive())
		{
			m_pExplosion->RenderShapes(m_giRimProgram);
		}

	//if game is paused, render menu shapes
		if (m_bPaused)
		{
			m_pPlayBtn->RenderShapes(m_giStaticProgram);
			m_pQuitBtn->RenderShapes(m_giStaticProgram);
			m_pHidden->RenderShapes(m_giStaticProgram);
		}

		break;

//lose game scenario
	case GameState::LOSE:
	//render game over screen shapes
		m_giCubeMap->Render(m_giCubeMapProgram);
		m_pMenuBackground->RenderShapes(m_giStaticProgram);
		m_pQuitBtn->RenderShapes(m_giStaticProgram);
		m_pRetryBtn->RenderShapes(m_giStaticProgram);
		scoreText->SetColor(vec3(200.0f, 200.0f, 255.0f));
		scoreText->SetText("Final Score: " + to_string(m_iscore));
		scoreText->SetPosition(vec2(-150.0f, 0.0f));
	//UI render
		scoreText->Render();
		hiscoreText->Render();
	//when lost once, set hiscore to show in game
		m_bhiScores = true;
		break;
	default:
		break;
	}
//always render mouse pointer shape
	m_pPointer->RenderShapes(m_giStaticProgram);

	glutSwapBuffers();
	//glFinish();
}
//update function
void CGameManager::Update()
{
//Update Time components
	m_pTime->UpdateTime();
	
//Get user input into proces input function
	ProcessInput(m_pInput->GetKeyState(), m_pInput->GetMouseState());

//switch for game state
	switch (m_eGameState)
	{
	case GameState::MENU:
	//menu component updates
		m_pProjCamera->CameraRotate(m_pTime);
		m_giCubeMap->Update();
		m_pMenuBackground->UpdateShapes();
		m_pPlayBtn->UpdateShapes();
		m_pCreditsBtn->UpdateShapes();
		m_pQuitBtn->UpdateShapes();
		m_pCreditsLayer->UpdateShapes();
		m_pPointer->SetObjPosition((float)m_pInput->GetMouseX(), (float)m_pInput->GetMouseY(),  0.0f);
		break;

			///*** GAME UPDATE *** ///

	case GameState::GAME:
		
		//If game is not paused - update shapes
		if (!m_bPaused)
		{
			//Poiner object follows the mouse at half speed
			m_pPointer->SetObjPosition((float)m_pInput->GetMouseX() * 0.5f, -10.0f, (float)m_pInput->GetMouseY() * 0.5f);

			//move player on input
			PlayerRun();
			//Set player to check hit and lose health when hit
			m_pHealthBar->LoseHealth();
			//healthbar update
			m_pHealthBar->UpdateShapes();
			//update cubemap
			m_giCubeMap->Update();
			//update player
			m_pPlayerPrefab->UpdateShapes(NULL, m_pPointer);
			//update arena floor
			m_pArena->UpdateShapes();
			//update score
			m_pScore->UpdateShapes();
			//update score text
			scoreText->SetText(to_string(m_iscore));
			//update UI frame
			m_pWalls->UpdateShapes();

			//random powerup spawner (1 at a time)
			if (!m_bspawnPup)
			{
				PowerUpRand();
			}
			else if (m_bspawnPup)
			{
				//check collision for powerup
				PlayerPowerupCollision();
			}

			//spawn enemies
			EnemySpawner();

			//update enemy
			for (size_t i = 0; i < m_vEnemySpriteVec.size(); i++)
			{
				m_vEnemySpriteVec[i]->UpdateShapes(m_pTime);
				//enemy group for separation ai
				m_vEnemySpriteVec[i]->SetGroup(m_vEnemySpriteVec);
				//target for seek ai
				m_vEnemySpriteVec[i]->SetTarget(vec2(m_pPlayerPrefab->GetObjPosition().x, m_pPlayerPrefab->GetObjPosition().z));
			}
			//update bullets
			for (size_t i = 0; i < m_vBullets.size(); i++)
			{
				if (m_vBullets[i]->GetDraw())
				{
					m_vBullets[i]->UpdateShapes();
					m_vBullets[i]->SetObjectVector();
				}
			}

			//update disco ball
			for (size_t i = 0; i < m_vDiscoVec.size(); i++)
			{
				m_vDiscoVec[i]->UpdateShapes(m_giCubeMap, m_pPointer);
				//change light colour over time
				m_vDiscoVec[i]->SetLightColour(vec3((sin(m_pTime->GetTime()) + 10), (cos(m_pTime->GetTime()) + 5) / 2, (sin(m_pTime->GetTime())) / 2));
			}
			//spawn powerup
			if (m_bspawnPup)
			{

				m_pPowerUp->UpdateShapes();
				
			}
			//on powerup active, active explode
			if (m_pExplosion->GetActive())
			{
				m_pExplosion->UpdateShapes();
			}

			//player powerup collision
			PlayerPowerupCollision();

			//kill enemies when hit
			EnemyDeathCheck();

			//player enemy collision
			PlayerEnemyCollision();

			//enemy bullet collision
			BulletEnemyCollision();

			//check for lose state
			if (m_pPlayerPrefab->GetDead())
			{
				m_blose = true;
				//change game state
				m_eGameState = GameState::LOSE;
				//clear game items
				ClearGame();
				//initialise gameover items
				InitialiseGameover();
			}
		}
		//if paused, stop updating all other shapes
		else
		{
			m_pPointer->SetObjPosition((float)m_pInput->GetMouseX(), (float)m_pInput->GetMouseY(), 0.0f);
			m_pPlayBtn->UpdateShapes();
			m_pQuitBtn->UpdateShapes();
			m_pHidden->UpdateShapes();
		}

		break;
	case GameState::LOSE:
		//get lose state active
		if (m_blose)
		{
			if (m_ihiscore < m_iscore)
			{
				m_ihiscore = m_iscore;
				hiscoreText->SetText("Hi Score: " + to_string(m_ihiscore));
			}
			PlaySounds(m_pFxScream);
			//do all this once
			m_blose = false;
		}
		//update game over shapes
		m_pProjCamera->CameraRotate(m_pTime);
		m_giCubeMap->Update();
		m_pMenuBackground->UpdateShapes();
		m_pPointer->SetObjPosition((float)m_pInput->GetMouseX(), (float)m_pInput->GetMouseY(),  0.0f);
		m_pQuitBtn->UpdateShapes();
		m_pRetryBtn->UpdateShapes();
		break;
	default:
		break;
	}

	//update mouse pointer prefab
	m_pPointer->UpdateShapes();
	
	//update audio
	m_pAudioSystem->GetAudioSystem()->update();

	//update game information
	glutPostRedisplay();
}
//player movement function
void CGameManager::PlayerRun()
{
	//move player based on input * player speed
	m_pPlayerPrefab->PlayerMovement(vec3(m_fhorizontal * m_fplayerSpeed, 0.0f, m_fvertical * m_fplayerSpeed), m_pTime);
}
//enemy spawner function
void CGameManager::EnemySpawner()
{
	//dont spawn more enemies than spawn size
	if ((int)m_vEnemySpriteVec.size() < m_ispawnSize)
	{
		//spawn new enemies on spawnRate tick
		if (m_fspawnCount > m_fspawnRate)
		{
			//initialise a new enemy
			CEnemy* enemySprite = new CEnemy((int)m_pPlayerPrefab->GetObjPosition().x, (int)m_pPlayerPrefab->GetObjPosition().z);
			//choose randomn location on screennear player
			srand((int)(rand() ^ time(NULL)));
			float arand = rand() % 100 - 65.0f;
			enemySprite->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::EMPTY, "", 11.0F, vec3(0.1f, 0.1f, 0.1f), vec3(0.0f, 0.0f, 0.0f), vec3((int)(m_pPlayerPrefab->GetObjPosition().x + arand), 45.0f, (int)(m_pPlayerPrefab->GetObjPosition().z + arand)));
			
			//create new empty model pointer for enemy
			CModel* enemyModelCopy = new CModel(m_pProjCamera);
			//set model copy to copy pre loded model mesh
			enemyModelCopy->SetMesh(m_pEnemyModel->GetMesh());
			enemyModelCopy->SetTexture(m_pEnemyModel->GetTextures());
			//send model copy into enemy prefab
			enemySprite->SetModel(enemyModelCopy);
		
			//initialise enemy animation and state
			enemySprite->InitialiseTextures("", 1);
			enemySprite->SetState(aniState::SPAWN);
			//add sprite to enemy vector
			m_vEnemySpriteVec.push_back(enemySprite);
			//reset spawn tick variable
			m_fspawnCount = 0.0f;
		}
		//increment spawn tick
		m_fspawnCount += m_pTime->GetDelta() * 100;
	}	
	//check if enemy is set to attacking and set to attack state
	for (size_t i = 0; i < m_vEnemySpriteVec.size(); i++)
	{
		if (m_vEnemySpriteVec[i]->GetAttacking())
		{
			m_vEnemySpriteVec[i]->SetState(aniState::ATTACK);
		}
	}
}
//Bullet and enemy collision
void CGameManager::BulletEnemyCollision()
{
	for (size_t j = 0; j < m_vEnemySpriteVec.size(); j++)
	{
		for (size_t i = 0; i < m_vBullets.size(); i++)
		{
			//check bullet and enemy collision
			if (m_vBullets[i]->CheckModelCollision(m_vEnemySpriteVec[j]) && m_vEnemySpriteVec[j]->GetState() != aniState::DEATH && m_vEnemySpriteVec[j]->GetState() != aniState::SPAWN)
			{
				//increment score
				m_iscore += 10;
				//if the spawn rate is 20 or more reduce the spawn rate (min 20)
				if (m_fspawnRate > 20)
				{
					m_fspawnRate -= m_pTime->GetDelta() * 50;
				}
				//set enemy sprite to death state
				m_vEnemySpriteVec[j]->SetState(aniState::DEATH);
				//play death sound
				PlaySounds(m_pFxSplat);
				//stop drawing the bullet and delete it from the vector
				m_vBullets[i]->SetDraw(false);
				CBullet* ABullet = m_vBullets[i];
				m_vBullets.erase(m_vBullets.begin() + i);
				delete ABullet;
			}
		}
	}
}
//Player and enemy collision
void CGameManager::PlayerEnemyCollision()
{
	if (m_fHitTimer == 0 && !m_pPlayerPrefab->GetHit())
	{
		//check for collision with mesh
		for (size_t j = 0; j < m_vEnemySpriteVec.size(); j++)
		{
			if (m_pPlayerPrefab->CheckModelCollision(m_vEnemySpriteVec[j]) && m_vEnemySpriteVec[j]->GetState() == aniState::ATTACK)
			{
				m_fHitTimer++;
				//set enemy sprite to death state
				m_vEnemySpriteVec[j]->SetState(aniState::DEATH);
				//set player hit on collision
				m_pPlayerPrefab->SetHit(true);
				PlaySounds(m_pFxHit);
				//play death sound
				PlaySounds(m_pFxSplat);
			}
		}
		
	}
	else if (m_pPlayerPrefab->GetHit())
	{
		m_fHitTimer = 0;
	}
}
//Powerup function
void CGameManager::SpawnPowerUp()
{
	//spawn powerup on random x, y position
	srand((unsigned int)(rand() ^ time(NULL)));
	float xrand = rand() % 130 - 70.0f; //top/bottom
	float zrand = rand() % 165 - 85.0f; //left/right
	//set position at a random location in arena
	m_pPowerUp->SetObjPosition(xrand, 0.0f,  zrand);
}
//random powerup spawning
void CGameManager::PowerUpRand()
{
	//spawn powerup when ranD is greater than 0.999f (low chance)wwwwwww
	srand((unsigned int)(rand() ^ time(NULL)));
	float ranD = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (ranD > 0.999f)
	{
		//allow for rendoring and spawn a powerup
		m_bspawnPup = true;
		SpawnPowerUp();
	}
}
//Player and powerup collision
void CGameManager::PlayerPowerupCollision()
{
	//If player is over powerup
	if (m_pPlayerPrefab->CheckModelCollision(m_pPowerUp) && m_bspawnPup)
	{
		//reset spawn powerup variables
		m_bspawnPup = false;
		//set all enemies to death state and add score per enemy killed
		for (size_t j = 0; j < m_vEnemySpriteVec.size(); j++)
		{
			m_vEnemySpriteVec[j]->SetState(aniState::DEATH);
			m_iscore += 10;
		}
		//play sound fx and initilaise the explotion
		PlaySounds(m_pFxExplode);
		m_pExplosion->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 100.0f, vec3(10, 10, 10), vec3(), vec3(0.0f, -20.0f, 0.0f));
		m_pExplosion->InitialiseTextures();
		m_pExplosion->SetActive(true);
	}
}
//Delete enemy from vector when death state
void CGameManager::EnemyDeathCheck()
{
	for (size_t i = 0; i < m_vEnemySpriteVec.size(); i++)
	{
		//if death state
		if (m_vEnemySpriteVec[i]->GetDead())
		{
			CEnemy* aEnemy = m_vEnemySpriteVec[i];
			m_vEnemySpriteVec.erase(m_vEnemySpriteVec.begin() + i);
			delete aEnemy;
			m_fHitTimer = 0;

			//update enemy vector in each enemy for AI code
			for (size_t j = 0; j < m_vEnemySpriteVec.size(); j++)
			{
				m_vEnemySpriteVec[j]->SetGroup(m_vEnemySpriteVec);
			}
		}
		
	}
}
//input functions
void CGameManager::ProcessInput(InputState* KeyState, InputState* MouseState)
{
								/// *** MAIN MENU INPUT *** ///
	switch (m_eGameState)
	{
	case GameState::MENU:

		//if mouse click counter is 0 (one click at a time)
		if (m_fcounter == 0.0f)
		{

			//if credits screen is false
			if (m_pCreditsLayer->GetShowing() == false)
			{
				//play button collision
				if (m_pPlayBtn->CheckHover(m_pInput))
				{
					//if hover counter = 0.0f
					if (m_ihover == 0)
					{
						//play hover sound fx
						PlaySounds(m_pFxBtn);
						// increment hover counter so fx plays once
						m_ihover++;
					}
					//set button hover
					m_pPlayBtn->SetButton(true);

					//button clicked action
					if (MouseState[0] == InputState::INPUT_DOWN)
					{
						//play start game sound fx
						PlaySounds(m_pFxPlay);
						m_eGameState = GameState::GAME;
						//clear menu items
						ClearMenu();
						InitialiseGame();
						m_fcounter++;
					}
				}
				else if (m_pPlayBtn->GetShowing())
				{
					//set button to default state when not hovering
					m_pPlayBtn->SetButton(false);
					m_ihover = 0;
				}
				//credits button collision
				else if (m_pCreditsBtn->CheckHover(m_pInput))
				{
					//if hover counter = 0.0f
					if (m_ihover == 0)
					{
						//play hover sound fx
						PlaySounds(m_pFxBtn);
						// increment hover counter so fx plays once
						m_ihover++;
					}
					//set button hover
					m_pCreditsBtn->SetButton(true);
					//show credits layer
					if (MouseState[0] == InputState::INPUT_DOWN)
					{
						m_pCreditsLayer->SetButton(true);
						m_fcounter++;
						m_pCreditsBtn->SetButton(false);
						m_ihover = 0;
					}
				}
				else if (m_pCreditsBtn->GetShowing())
				{
					//set button to default state when not hovering
					m_pCreditsBtn->SetButton(false);
					m_ihover = 0;
				}
				//quit button collision
				else if (m_pQuitBtn->CheckHover(m_pInput))
				{
					//if hover counter = 0.0f
					if (m_ihover == 0)
					{
						//play hover sound fx
						PlaySounds(m_pFxBtn);
						// increment hover counter so fx plays once
						m_ihover++;
					}
					//set button hover
					m_pQuitBtn->SetButton(true);
					//quit game on click
					if (MouseState[0] == InputState::INPUT_DOWN)
					{
						exit(0);
					}
				}
				else
				{
					//set button to default state when not hovering
					m_pQuitBtn->SetButton(false);
					m_ihover = 0;
				}
			}
			//if credits layer is showing
			else if (m_pCreditsLayer->GetShowing() == true)
			{
				//hide layer on mouse click
				if (MouseState[0] == InputState::INPUT_DOWN)
				{
					m_pCreditsLayer->SetButton(false);
					m_fcounter++;
					m_ihover = 0;
				}
			}
		}
		else if (MouseState[0] == InputState::INPUT_UP)
		{
		//reset counter on mouse up
			m_fcounter = 0.0f;
		}
		break;

													// **** GAME INPUT **** //

	case GameState::GAME:

		//Menu


		//Bring up menu on escape key
		if (m_fcounter == 0.0f)
		{
			if (KeyState[27] == InputState::INPUT_DOWN)
			{
				m_bPaused = true;
				Initialise2DPointer();
				m_fcounter++;
			}
		}
		else if (m_bPaused == true)
		{
			m_fcounter = 0.0f;
		}

		if (!m_bPaused)
		{

			// *** PLAYER MOVEMENT *** //

			//set up boundary collision for camera movement
			//Left collision
			if (m_pPlayerPrefab->GetObjPosition().x <= m_pProjCamera->GetCamLookDir().x - 90)
			{
				m_pProjCamera->MoveCamera(vec3(-1.0f, 0.0f, 0.0f), m_pTime);
			}	
			//Right collision
			else if (m_pPlayerPrefab->GetObjPosition().x >= m_pProjCamera->GetCamLookDir().x + 90)
			{
				m_pProjCamera->MoveCamera(vec3(1.0f, 0.0f, 0.0f), m_pTime);
			}
			//Top collision
			if (m_pPlayerPrefab->GetObjPosition().z <= m_pProjCamera->GetCamLookDir().z - 60)
			{
				m_pProjCamera->MoveCamera(vec3(0.0f, 0.0f, -1.0f), m_pTime);
			}
			else if (m_pPlayerPrefab->GetObjPosition().z >= m_pProjCamera->GetCamLookDir().z + 30)
			{
				m_pProjCamera->MoveCamera(vec3(0.0f, 0.0f, 1.0f), m_pTime);
			}

			
			//Level Boundary
			//Left wall collision
			if (m_pPlayerPrefab->GetObjPosition().x >= -Utils::HalfScreenW/2)
			{
				m_bgoingL = true;
			}
			else
			{
				m_bgoingL = false;
			}
			//Right wall collision
			if (m_pPlayerPrefab->GetObjPosition().x <= Utils::HalfScreenW / 2)
			{
				m_bgoingR = true;
			}
			else
			{
				m_bgoingR = false;
			}
			//Top wall collision
			if (m_pPlayerPrefab->GetObjPosition().z >= -Utils::HalfScreenH/2)
			{
				m_bgoingU = true;
			}
			else
			{
				m_bgoingU = false;
			}
			//Bottom wall collision
			if (m_pPlayerPrefab->GetObjPosition().z <= Utils::HalfScreenH / 2)
			{
				m_bgoingD = true;
			}
			else
			{
				m_bgoingD = false;
			}

			//going up or down on y axis
			if (m_bgoingU && KeyState['w'] == InputState::INPUT_DOWN)
			{
				m_fvertical = -1.0f;
			}
			else if (m_bgoingD && KeyState['s'] == InputState::INPUT_DOWN)
			{
				m_fvertical = 1.0f;
			}
			else
			{
				m_fvertical = 0;
			}
			//going left or right on x axis
			if (m_bgoingL && KeyState['a'] == InputState::INPUT_DOWN)
			{
				m_fhorizontal = -1.0f;
			}
			else if (m_bgoingR && KeyState['d'] == InputState::INPUT_DOWN)
			{
				m_fhorizontal = 1.0f;
			}
			else
			{
				m_fhorizontal = 0;
			}

			//mouse clicked
			if (MouseState[0] == InputState::INPUT_DOWN)
			{
				//spawn bullet on mouse click 
				if (m_fcounter == 0.0f)
				{
					CBullet* Bullet = new CBullet((int)m_pPointer->GetObjPosition().x, (int)m_pPointer->GetObjPosition().z);
					Bullet->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::SPHERE, "", 0, vec3(1.0, 1.0, 1.0f), vec3(), vec3(m_pPlayerPrefab->GetObjPosition().x, 0.0f, m_pPlayerPrefab->GetObjPosition().z));
					Bullet->InitialiseTextures("Resources/Textures/bullet.png", 1);
					m_vBullets.push_back(Bullet);
					PlaySounds(m_pFxBullet);
				}

				m_fcounter += (m_pTime->GetDelta() * 100);
				//simulate delay on fire
				if (m_fcounter >= 30.0f)
				{
					m_fcounter = 0.0f;
				}

			}
			else if (MouseState[0] == InputState::INPUT_UP)
			{
				m_fcounter = 0.0f;
			}
		}
		else 
		{
		
			if (m_fcounter == 0.0f)
			{
				
				
				//play button collision
				if (m_pPlayBtn->CheckHover(m_pInput))
				{
					//if hover counter = 0.0f
					if (m_ihover == 0)
					{
						m_pQuitBtn->SetButton(false);
						//play hover sound fx
						PlaySounds(m_pFxBtn);
						// increment hover counter so fx plays once
						m_ihover++;
					}
					//set button hover
					m_pPlayBtn->SetButton(true);

					//button clicked action
					if (MouseState[0] == InputState::INPUT_DOWN)
					{
						//play start game sound fx
						Initialise3DPointer();
						m_bPaused = false;
						m_fcounter++;
					}
				}
				else if (m_pPlayBtn->GetShowing())
				 {
					 //set button to default state when not hovering
					 m_pPlayBtn->SetButton(false);
					 m_ihover = 0;
				 }
				
				else if (m_pQuitBtn->CheckHover(m_pInput))
				{
					//if hover counter = 0.0f
					if (m_ihover == 0)
					{
						m_pPlayBtn->SetButton(false);
						//play hover sound fx
						PlaySounds(m_pFxBtn);
						// increment hover counter so fx plays once
						m_ihover++;
					}
					//set button hover
					m_pQuitBtn->SetButton(true);
					//exit game on click
					if (MouseState[0] == InputState::INPUT_DOWN)
					{
						exit(0);
					}
				}
				else  if (m_pQuitBtn->GetShowing())
			{
				m_pQuitBtn->SetButton(false);
				m_ihover = 0;
			}
				
			}
		}

		break;

									/// *** LOSE MENU INPUT *** ///
	case GameState::LOSE:
		if (m_fcounter == 0.0f)
		{
			//retry button collision
			if (m_pRetryBtn->CheckHover(m_pInput))
			{
				//if hover counter = 0.0f
				if (m_ihover == 0)
				{
					//play hover sound fx
					PlaySounds(m_pFxBtn);
					// increment hover counter so fx plays once
					m_ihover++;
				}
				//set button hover
				m_pRetryBtn->SetButton(true);
				//reset game on click
				if (MouseState[0] == InputState::INPUT_DOWN)
				{
					PlaySounds(m_pFxPlay);
					m_eGameState = GameState::GAME;
					ClearGameover();
					InitialiseGame();
					m_iscore = 0;
				}
			}
			else if (m_pRetryBtn->GetShowing())
			{
				m_pRetryBtn->SetButton(false);
				m_ihover = 0;
			}
			//quit button collision
			else if (m_pQuitBtn->CheckHover(m_pInput))
			{
				//if hover counter = 0.0f
				if (m_ihover == 0)
				{
					//play hover sound fx
					PlaySounds(m_pFxBtn);
					// increment hover counter so fx plays once
					m_ihover++;
				}
				//set button hover
				m_pQuitBtn->SetButton(true);
				//exit game on click
				if (MouseState[0] == InputState::INPUT_DOWN)
				{
					exit(0);
				}
			}
			else
			{
				m_pQuitBtn->SetButton(false);
				m_ihover = 0;
			}
		}
		else if (MouseState[0] == InputState::INPUT_UP)
		{
			m_fcounter = 0.0f;
		}
		break;
	default:
		break;
	}

}
//set pointer 2D
void CGameManager::Initialise2DPointer()
{
	delete m_pPointer;
	m_pPointer = 0;

	m_pPointer = new CPrefab();
	//pointer 2D shape
	m_pPointer->Initialise(m_pOrthoCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(40.0, 40.0, 0));
	m_pPointer->InitialiseTextures("Resources/Textures/pointer.png", 1);
}
//set pointer 3D
void CGameManager::Initialise3DPointer()
{
	delete m_pPointer;
	m_pPointer = 0;

	m_pPointer = new CPrefab();
	//pointer with 3D shape
	m_pPointer->Initialise(m_pProjCamera, m_pTime, m_pInput, MeshType::QUAD, "", 0, vec3(10.0f, 10.0f, 0.0f), vec3(90.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
	m_pPointer->InitialiseTextures("Resources/Textures/pointer.png", 1);
}