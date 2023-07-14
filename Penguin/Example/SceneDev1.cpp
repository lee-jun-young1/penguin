#include "stdafx.h"
#include "SceneDev1.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <ResourceManager.h>
#include <SpriteGO.h>
#include <Framework.h>
#include "VertexArrayGO.h"
#include <Utils.h>
#include "TextGameObj.h"
#include "RectangleShapeGO.h"
#include <sstream>
#include "SoundGO.h"

SceneDev1::SceneDev1()
	:Scene(SceneId::Dev1)
{
}

SceneDev1::~SceneDev1()
{
	Release();
}

void SceneDev1::Enter()
{
	Scene::Enter();
}

void SceneDev1::Reset()
{

	for (auto go : gameObjects)
	{
		go->Reset();
	}
	
}

void SceneDev1::Exit()
{
	Scene::Exit();
}					 

void SceneDev1::Init()
{
	Release();

	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	sf::Vector2f centerPos = windowSize * 0.5f;

	worldView.setSize(windowSize);
	worldView.setCenter(0.0f, 0.0f);
	uiView.setSize(windowSize);
	uiView.setCenter(centerPos);

	for (auto go : gameObjects)
	{
		go->Init();
	}

}

void SceneDev1::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}

	//resources.clear();
}

void SceneDev1::Update(float deltaTime)
{
	Scene::Update(deltaTime);
}

void SceneDev1::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

template<typename T>
inline void SceneDev1::ClearObjectPool(ObjectPool<T>& pool) 
{
	for (auto obj : pool.GetUseList()) 
	{
		RemoveGameObject(obj); 
	}
	pool.Clear(); 
}