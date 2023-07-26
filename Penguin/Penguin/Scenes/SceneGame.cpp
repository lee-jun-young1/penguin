#include "stdafx.h"
#include <Framework.h>
#include "SceneGame.h"
#include <SpriteFont.h>
#include <SpriteTextGO.h>
#include <Animator.h>
#include <Penta.h>
#include <RigidBody2D.h>
#include <BoxCollider.h>
#include <RectangleShapeGO.h>
#include <SlicedSpriteGO.h>
#include <IceHole.h>
#include <Crevasse.h>
#include <Seal.h>
#include <Background.h>
#include <PentaShadow.h>

SceneGame::SceneGame() 
	: Scene(SceneId::Game)
{
	sceneName = "GameScene";
}

SceneGame::~SceneGame()
{
	//Release();
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	auto screenCenter = size * 0.5f;
	uiView.setSize(size);
	uiView.setCenter(screenCenter.x, screenCenter.y);
	screenCenter.y -= 60.0f;
	worldView.setSize(size);
	worldView.setCenter(screenCenter);


	Resources.Load(resources);

	Reset();

	for (auto go : gameObjects)
	{
		go->Reset();
	}
	stageManager->ResetScore();
	stageManager->SetStage(1);
	//test->SetTexture(*RESOURCE_MANAGER.GetTexture("graphics/SliceSpriteTest.png"), { 10, 6, 20, 2 }, { 0, 0, 40, 13 });
}

void SceneGame::Reset()
{
	SpriteFont* font = Resources.GetSpriteFont("fonts/SpriteFont_Data.csv");

	SpriteTextGO* demoText = (SpriteTextGO*)FindGameObject("DemoText");
	demoText->SetFont(font);
	demoText->SetText("ANTARCTIC ADVENTURE DEMO");
	demoText->SetPosition({ 15.0f, 15.0f });


	SpriteTextGO* scoreText = (SpriteTextGO*)FindGameObject("ScoreText");
	scoreText->SetFont(font);
	scoreText->SetText("1P-");
	scoreText->SetPosition({ 15.0f, 30.0f });

	SpriteTextGO* highScoreText = (SpriteTextGO*)FindGameObject("HighScoreText");
	highScoreText->SetFont(font);
	highScoreText->SetText("HI-000000");
	highScoreText->SetPosition({ 95.0f, 30.0f });


	SpriteTextGO* timeText = (SpriteTextGO*)FindGameObject("TimeText");
	timeText->SetFont(font);
	timeText->SetText("TIME-");
	timeText->SetPosition({ 15.0f, 40.0f });

	SpriteTextGO* restText = (SpriteTextGO*)FindGameObject("RestText");
	restText->SetFont(font);
	restText->SetText("wxyz_1139km");
	restText->SetPosition({ 95.0f, 40.0f });


	SpriteTextGO* speedText = (SpriteTextGO*)FindGameObject("SpeedText");
	speedText->SetFont(font);
	speedText->SetText("speed");
	speedText->SetPosition(175.0f, 40.0f);


	SpriteTextGO* stageText = (SpriteTextGO*)FindGameObject("StageText");
	stageText->SetFont(font);
	stageText->SetText("STAGE-00");
	stageText->SetPosition(175.0f, 30.0f);

	SpriteTextGO* sysMsgText = (SpriteTextGO*)FindGameObject("SysMsg");
	sysMsgText->SetFont(font);
	sysMsgText->SetText("TIME OUT");
	sysMsgText->SetPosition(UIPositionToScreen({ FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.3f }));
	sysMsgText->SetOrigin(Origins::MC);
	sysMsgText->SetActive(false);

	RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)FindGameObject("SysMsgRect");
	sysMsgRect->SetFillColor(sf::Color::Black);
	sysMsgRect->SetSize(sysMsgText->GetSize());
	sysMsgRect->SetPosition(UIPositionToScreen(sysMsgText->GetPosition()));
	sysMsgRect->SetOrigin(Origins::MC);
	sysMsgRect->SetActive(false);

	for (auto go : gameObjects)
	{
		go->Reset();
	}
	AudioSource* bgm = (AudioSource*)FindGameObject("Background")->GetComponent(ComponentType::Audio);
	bgm->SetClip(Resources.GetSoundBuffer("sound/bg/2_MainBgm.ogg"));
	bgm->SetLoop(true);
	bgm->Play();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Init()
{
	Scene::Init();
	Release();

	Background* bg = (Background*)AddGameObject(new Background("graphics/BG.png", "Background"));
	bg->sortLayer = -1;
	AudioSource* bgm = new AudioSource(*bg);
	bg->AddComponent(bgm);


	stageManager = (StageManager*)AddGameObject(new StageManager());



	RectangleShapeGO* ground = (RectangleShapeGO*)AddGameObject(new RectangleShapeGO("Ground"));
	ground->physicsLayer = (int)PhysicsLayer::Ground;
	ground->sortLayer = -1;
	BoxCollider* boxCol = new BoxCollider(*ground);
	ground->SetSize({ FRAMEWORK.GetWindowSize().x, 40.0f });
	ground->SetPosition({ 0.0f, 165.0f });
	boxCol->SetRect({ 0.0f, 165.0f, FRAMEWORK.GetWindowSize().x, 100.0f });
	ground->AddComponent(boxCol);



	Penta* player = (Penta*)AddGameObject(new Penta("graphics/Penta.png", "Player"));
	player->sortLayer = 2;
	player->physicsLayer = (int)PhysicsLayer::Player;
	PentaShadow* shadow = (PentaShadow*)AddGameObject(new PentaShadow());
	shadow->SetPenta(player);
	player->SetShadow(shadow);

	Animator* animator = new Animator(*player, "animations/Penta", "Move");
	player->AddComponent(animator);
	player->SetAnimator(animator);
	RigidBody2D* playerRig = new RigidBody2D(*player);
	playerRig->SetGravity(false);
	player->SetRigidBody(playerRig);
	player->AddComponent(playerRig);
	BoxCollider* playerCol = new BoxCollider(*player);
	playerCol->SetRigidbody(playerRig);
	player->AddComponent(playerCol);
	player->SetPosition(100.0f, 100.0f);

	SpriteGO* pegicopter = (SpriteGO*)AddGameObject(new SpriteGO("graphics/Pegicopter.png", "Player"));
	pegicopter->sortLayer = 3;
	Animator* pegicopterAni = new Animator(*pegicopter, "animations/Pegicopter", "Idle");
	pegicopter->AddComponent(pegicopterAni);
	player->SetPegicopter(pegicopter, pegicopterAni);


	GameObject* wall = (GameObject*)AddGameObject(new GameObject("Wall"));
	wall->physicsLayer = (int)PhysicsLayer::Wall;
	wall->SetPosition({ 0.0f, 0.0f });

	BoxCollider* leftWallCol = new BoxCollider(*wall);
	leftWallCol->SetRect({ 0.0f, 0.0f, 10.0f, FRAMEWORK.GetWindowSize().y });
	wall->AddComponent(leftWallCol);

	BoxCollider* rightWallCol = new BoxCollider(*wall);
	rightWallCol->SetRect({ 0.0f, 0.0f,  10.0f, FRAMEWORK.GetWindowSize().y });
	rightWallCol->SetOffset({ FRAMEWORK.GetWindowSize().x - 10.0f, 0.0f });
	sf::Vector2f offset = rightWallCol->GetOffset();
	wall->AddComponent(rightWallCol);


	SpriteTextGO* demoText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("DemoText"));
	demoText->sortLayer = UILayer;


	SpriteTextGO* scoreText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("ScoreText"));
	scoreText->sortLayer = UILayer;

	SpriteTextGO* highScoreText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("HighScoreText"));
	highScoreText->sortLayer = UILayer;


	SpriteTextGO* timeText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("TimeText"));
	timeText->sortLayer = UILayer;

	stageManager->SetTimeText(timeText);

	SpriteTextGO* restText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("RestText"));
	restText->sortLayer = UILayer;

	stageManager->SetRestText(restText);


	SpriteTextGO* speedText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("SpeedText"));
	speedText->sortLayer = UILayer;


	SpriteTextGO* stageText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("StageText"));
	stageText->sortLayer = UILayer;

	SpriteTextGO* sysMsgText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("SysMsg"));
	sysMsgText->sortLayer = UILayer;
	sysMsgText->sortOrder = 1;

	RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)AddGameObject(new RectangleShapeGO("SysMsgRect"));
	sysMsgRect->sortLayer = UILayer;
	sysMsgRect->sortOrder = 0;

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneGame::Update(float deltaTime)
{
	Scene::Update(deltaTime);

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

StageManager* SceneGame::GetStageManager()
{
	return stageManager;
}
