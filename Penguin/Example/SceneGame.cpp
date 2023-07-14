#include "stdafx.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "GameObject.h"

SceneGame::SceneGame()
	:Scene(SceneId::Dev2)
{
}

void SceneGame::Enter()
{
	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Init()
{
	Release();

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
	if (INPUT.GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MANAGER.ChangeScene(SceneId::Dev1);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
