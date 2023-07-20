#include "stdafx.h"
#include "IceHole.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Penta.h>
#include <SceneManager.h>
#include <SceneGame.h>
#include <Utils.h>
#include "ObstacleManager.h"

IceHole::IceHole(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size, "IceHole")
{
}

void IceHole::SetManager(ObstacleManager* manager)
{
	this->manager = manager;
}

void IceHole::OnTriggerEnter(Collider* col)
{
}

void IceHole::OnTriggerStay(Collider* col)
{
}

void IceHole::OnTriggerExit(Collider* col)
{
	if (col->GetGameObject().GetName() == "Ground")
	{
 		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetObstacleManager()->ReturnIceHole(this);
	}
}

void IceHole::SetDirection(sf::Vector2f startPos, sf::Vector2f endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}

void IceHole::Init()
{
	SlicedSpriteGO::Init();
	physicsLayer = 5;
	collider = new BoxCollider(*this);
	collider->SetTrigger(true);
	AddComponent(collider);
}

void IceHole::Reset()
{
	SlicedSpriteGO::Reset();
	size = sizeMin;
	SetSize(size);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.4f, 60.0f });
	collider->SetEnable(false);
	SetOrigin(Origins::BC);
}

void IceHole::Update(float dt)
{
	time += dt * manager->GetSpeed();

	size = Utils::Lerp(sizeMin, sizeMax, time, false);
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));
	if (position.y > 160.0f)
	{
		collider->SetEnable(true);
	}
	SlicedSpriteGO::Update(dt);
	if (seal != nullptr)
	{
		//seal->SetPosition({position.x + size.x * 0.5f,  position.y + size.y * 0.5f});
		seal->SetPosition({ position.x,  position.y - size.y * 0.5f });
	}

	if (FRAMEWORK.GetWindowSize().x + size.y < position.y)
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetObstacleManager()->ReturnIceHole(this);
	}
}

void IceHole::SetSize(sf::Vector2f size)
{
	SlicedSpriteGO::SetSize(size);
	SetOrigin(Origins::BC);
	collider->SetRect({ position.x, position.y, size.x, size.y });
	collider->SetOffset({ size.x * -0.5f, -size.y });
}

void IceHole::SetSeal(Seal* seal)
{
	this->seal = seal;
}

Seal* IceHole::GetSeal()
{
	return seal;
}
