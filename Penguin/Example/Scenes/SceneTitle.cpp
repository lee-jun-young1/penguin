#include "stdafx.h"
#include <Framework.h>
#include "SceneTitle.h"
#include <SpriteFont.h>
#include <GameObjects/SpriteTextGO.h>
#include <Animator.h>
#include <AnimatorPlayer.h>
#include <RigidBody2D.h>
#include <BoxCollider.h>
#include <GameObjects/RectangleShapeGO.h>

SceneTitle::SceneTitle() 
	: Scene(SceneId::Title)
{
	sceneName = "TitleScene";
}

SceneTitle::~SceneTitle()
{
	Release();
}

void SceneTitle::Enter()
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
}

void SceneTitle::Reset()
{

	for (auto go : gameObjects)
	{
		go->Reset();
	}
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Init()
{
	Scene::Init();
	Release();

	SpriteGO* bg = (SpriteGO*)AddGameObject(new SpriteGO("graphics/tempBG.png", ""));
	bg->sortLayer = -1;

	//SpriteFont* font = new SpriteFont("fonts/SpriteFont_Data.csv");
	//SpriteTextGO* st = (SpriteTextGO*)AddGameObject(new SpriteTextGO());
	//st->SetFont(font);
	//st->SetText("ANTARCTIC ADVENTURE");

	AnimatorPlayer* test = (AnimatorPlayer*)AddGameObject(new AnimatorPlayer("graphics/Penta.png", "Player"));
	Animator* animator = new Animator(*test);
	test->AddComponent(animator);
	test->SetAnimator(animator);
	RigidBody2D* playerRig = new RigidBody2D(*test);
	test->AddComponent(playerRig);
	BoxCollider* playerCol = new BoxCollider(*test);
	playerCol->SetRigidbody(playerRig);
	test->AddComponent(playerCol);
	test->SetPosition(100.0f, 100.0f);

	RectangleShapeGO* ground = (RectangleShapeGO*)AddGameObject(new RectangleShapeGO());
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

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Update(float deltaTime)
{
	Scene::Update(deltaTime);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
