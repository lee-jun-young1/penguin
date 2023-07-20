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


	Scene::Enter();
	Reset();


	//test->SetTexture(*RESOURCE_MANAGER.GetTexture("graphics/SliceSpriteTest.png"), { 10, 6, 20, 2 }, { 0, 0, 40, 13 });
}

void SceneGame::Reset()
{
	for (auto go : gameObjects)
	{
		go->Reset();
	}
	AudioSource* bgm = (AudioSource*)FindGameObject("Background")->GetComponent(ComponentType::Audio);
	bgm->SetClip(RESOURCE_MANAGER.GetSoundBuffer("sound/bg/2_MainBgm.ogg"));
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

	SpriteGO* bg = (SpriteGO*)AddGameObject(new SpriteGO("graphics/tempBG.png", "Background"));
	bg->sortLayer = -1;
	AudioSource* bgm = new AudioSource(*bg);
	bg->AddComponent(bgm);


	Penta* player = (Penta*)AddGameObject(new Penta("graphics/Penta.png", "Player"));
	player->sortLayer = 2;
	player->physicsLayer = (int)PhysicsLayer::Player;
	Animator* animator = new Animator(*player);
	player->AddComponent(animator);
	player->SetAnimator(animator);
	RigidBody2D* playerRig = new RigidBody2D(*player);
	player->SetRigidBody(playerRig);
	player->AddComponent(playerRig);
	BoxCollider* playerCol = new BoxCollider(*player);
	playerCol->SetRigidbody(playerRig);
	player->AddComponent(playerCol);
	player->SetPosition(100.0f, 100.0f);



	RectangleShapeGO* ground = (RectangleShapeGO*)AddGameObject(new RectangleShapeGO("Ground"));
	ground->physicsLayer = (int)PhysicsLayer::Ground;
	BoxCollider* boxCol = new BoxCollider(*ground);
	ground->SetSize({ FRAMEWORK.GetWindowSize().x, 40.0f });
	ground->SetPosition({ 0.0f, 165.0f });
	boxCol->SetRect({ 0.0f, 165.0f, FRAMEWORK.GetWindowSize().x, 100.0f });
	ground->AddComponent(boxCol);



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


	obstacleManager = (ObstacleManager*)AddGameObject(new ObstacleManager());

	SpriteFont* font = new SpriteFont("fonts/SpriteFont_Data.csv");
	SpriteTextGO* speedText = (SpriteTextGO*)AddGameObject(new SpriteTextGO("SpeedText"));
	speedText->SetFont(font);
	speedText->SetText("SPEED");
	speedText->SetPosition(FRAMEWORK.GetWindowSize().x - 80.0f, -20.0f);

	SpriteTextGO* st = (SpriteTextGO*)AddGameObject(new SpriteTextGO());
	st->SetFont(font);
	st->SetText("ANTARCTIC ADVENTURE DEMO");
	st->SetPosition(15.0f, -45.0f);

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

ObstacleManager* SceneGame::GetObstacleManager()
{
	return obstacleManager;
}
