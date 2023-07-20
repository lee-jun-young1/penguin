#include "stdafx.h"
#include "Crevasse.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>
#include <SceneGame.h>
#include <SceneManager.h>
#include "ObstacleManager.h"

Crevasse::Crevasse(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size)
{
}

void Crevasse::SetManager(ObstacleManager* manager)
{
	this->manager = manager;
}

void Crevasse::Init()
{
	physicsLayer = 5;

	center = new CrevasseCenter("CrevasseCenter");
	center->physicsLayer = 5;
	center->SetParent(this);
	centerCol = new BoxCollider(*center);
	centerCol->SetTrigger(true);
	centerCol->SetRect({ 20.0f, 0.0f, 40.0f, 8.0f });
	centerCol->SetOffset({ -20.0f, 0.0f });
	center->AddComponent(centerCol);

	side = new GameObject("CrevasseSide");
	side->SetParent(this);
	side->physicsLayer = 5;
	leftCol = new BoxCollider(*side);
	leftCol->SetTrigger(true);
	leftCol->SetRect({ 0.0f, 0.0f, 10.0f, 8.0f });
	leftCol->SetOffset({ -40.0f, 0.0f });
	rightCol = new BoxCollider(*side);
	rightCol->SetTrigger(true);
	rightCol->SetRect({ 90.0f, 0.0f, 10.0f, 8.0f });
	rightCol->SetOffset({ 40.0f, 0.0f });
	side->AddComponent(leftCol);
	side->AddComponent(rightCol);
}

void Crevasse::Release()
{
	delete center;
	delete side;
}

void Crevasse::Reset()
{
	SlicedSpriteGO::Reset();
	size = sizeMin;
	SetSize(size);
	SetOrigin(Origins::BC);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.6f, 60.0f });
	time = 0.0f;
	center->SetActive(false);
	side->SetActive(false);
}

void Crevasse::SetDirection(sf::Vector2f startPos, sf::Vector2f endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}

void Crevasse::Update(float dt)
{
	time += dt * manager->GetSpeed();

	size = Utils::Lerp(sizeMin, sizeMax, time, false);
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));
	
	if (time > 1.0f)
	{
		center->SetPosition(position);
		side->SetPosition(position);
		center->SetActive(true);
		side->SetActive(true);
		center->Update(dt);
		side->Update(dt);
	}
}

void Crevasse::OnGUI(sf::RenderWindow& window)
{
#ifdef _DEBUG
	center->OnGUI(window);
	side->OnGUI(window);
#endif
}

void Crevasse::Draw(sf::RenderWindow& window)
{
	SlicedSpriteGO::Draw(window);
}

void Crevasse::OnTriggerEnter(Collider* col)
{
}

void Crevasse::OnTriggerStay(Collider* col)
{
}

void Crevasse::OnTriggerExit(Collider* col)
{
	if (col->GetGameObject().GetName() == "Ground")
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetObstacleManager()->ReturnCrevasse(this);
	}
}

void Crevasse::SetSize(sf::Vector2f size)
{
	SlicedSpriteGO::SetSize(size);
	SetOrigin(Origins::BC);
	centerCol->SetRect({ position.x, position.y, size.x * 0.4f, size.y });
	centerCol->SetOffset({ size.x * -0.2f, -size.y });

	leftCol->SetRect({ position.x, position.y, size.x * 0.15f, size.y });
	leftCol->SetOffset({ size.x * -0.5f, -size.y });

	rightCol->SetRect({ position.x, position.y, size.x * 0.15f, size.y });
	rightCol->SetOffset({ size.x * 0.35f, -size.y });
}