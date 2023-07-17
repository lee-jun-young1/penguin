#include "stdafx.h"
#include <Framework.h>
#include "SceneGame.h"
#include <SpriteFont.h>
#include <GameObjects/SpriteTextGO.h>
#include <Animator.h>
#include <Penta.h>
#include <RigidBody2D.h>
#include <BoxCollider.h>
#include <GameObjects/RectangleShapeGO.h>
#include <SlicedSpriteGO.h>
#include <IceHole.h>

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

	IceHole* test = (IceHole*)AddGameObject(new IceHole());

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
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Init()
{
	Scene::Init();
	Release();

	SpriteGO* bg = (SpriteGO*)AddGameObject(new SpriteGO("graphics/tempBG.png", ""));
	bg->sortLayer = -1;

	//SpriteFont* font = new SpriteFont("fonts/SpriteFont_Data.csv");
	//SpriteTextGO* st = (SpriteTextGO*)AddGameObject(new SpriteTextGO());
	//st->SetFont(font);
	//st->SetText("ANTARCTIC ADVENTURE");

	Penta* player = (Penta*)AddGameObject(new Penta("graphics/Penta.png", "Player"));
	Animator* animator = new Animator(*player);
	player->AddComponent(animator);
	player->SetAnimator(animator);
	player->sortLayer = 2;
	RigidBody2D* playerRig = new RigidBody2D(*player);
	player->AddComponent(playerRig);
	BoxCollider* playerCol = new BoxCollider(*player);
	playerCol->SetRigidbody(playerRig);
	player->AddComponent(playerCol);
	player->SetPosition(100.0f, 100.0f);

	RectangleShapeGO* ground = (RectangleShapeGO*)AddGameObject(new RectangleShapeGO("Ground"));
	BoxCollider* boxCol = new BoxCollider(*ground);
	ground->SetSize({ FRAMEWORK.GetWindowSize().x, 100.0f });
	ground->SetPosition({ 0.0f, 150.0f });
	boxCol->SetRect({ 0.0f, 150.0f, FRAMEWORK.GetWindowSize().x, 100.0f });
	ground->AddComponent(boxCol);

	
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
