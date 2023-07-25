#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneDemo.h"
#include <PhysicsManager.h>

void SceneManager::ChangeScene(SceneId id)
{
	currentScene->Exit();

	Physics.Clear();
	currentSceneId = id;
	currentScene = scenes[(int)currentSceneId];

	currentScene->Enter();
}

void SceneManager::Init()
{
	if (!scenes.empty())
	{
		Release();
	}
	scenes.push_back(new SceneTitle());
	scenes.push_back(new SceneDemo());
	scenes.push_back(new SceneGame());

	for (auto scene : scenes)
	{
		scene->Init();
	}

	currentSceneId = startSceneId;
	currentScene = scenes[(int)currentSceneId];
	currentScene->Enter();
}

void SceneManager::Release()
{
	if (scenes.empty())
	{
		return;
	}
	for (auto scene : scenes)
	{
		delete scene;
	}
	scenes.clear();

	currentSceneId = SceneId::None;
	currentScene = nullptr;
}

void SceneManager::UpdateComponent(float deltaTime)
{
	scenes[(int)currentSceneId]->UpdateComponent(deltaTime);
}

void SceneManager::Update(float deltaTime)
{
	scenes[(int)currentSceneId]->Update(deltaTime);
}

void SceneManager::Draw(sf::RenderWindow& window)
{
	scenes[(int)currentSceneId]->Draw(window);
}

void SceneManager::OnGUI(sf::RenderWindow& window)
{
	scenes[(int)currentSceneId]->OnGUI(window);
}

Scene* SceneManager::GetCurrentScene() const
{
	return currentScene;
}
