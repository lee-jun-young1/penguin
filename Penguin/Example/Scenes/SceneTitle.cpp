#include "stdafx.h"
#include <Framework.h>
#include "SceneTitle.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "SoundGO.h"
#include <SpriteGO.h>
#include "TextGameObj.h"
#include <DataTableManager.h>
#include <StringTable.h>
#include "Player.h"
#include <RectangleShapeGO.h>
#include "RigidBody2D.h"
#include "BoxCollider.h"
#include <Player_2.h>
#include <UIButton.h>
#include <SpriteTextGO.h>
#include <AnimatorPlayer.h>
#include <AudioSource.h>

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
	worldView.setSize(size);
	worldView.setCenter(0.0f, 0.0f);

	uiView.setSize(size);
	uiView.setCenter(screenCenter.x, screenCenter.y);

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

	SpriteFont* font = new SpriteFont("fonts/SpriteFont_Data.csv");
	SpriteTextGO* st = (SpriteTextGO*)AddGameObject(new SpriteTextGO());
	st->SetFont(font);
	st->SetText("ANTARCTIC ADVENTURE");

	AnimatorPlayer* test = (AnimatorPlayer*)AddGameObject(new AnimatorPlayer("", "Player"));
	Animator* animator = new Animator(*test);
	test->AddComponent(animator);
	test->SetAnimator(animator);

	
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
