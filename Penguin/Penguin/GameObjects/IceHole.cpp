#include "stdafx.h"
#include "IceHole.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Penta.h>
#include <SceneManager.h>
#include <SceneGame.h>
#include <Utils.h>
#include "StageManager.h"

IceHole::IceHole(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size, "IceHole")
{
}

void IceHole::SetManager(StageManager* manager)
{
	this->manager = manager;
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
	time = 0;
	SlicedSpriteGO::Reset();
	size = sizeMin;
	SetSize(size);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.4f, 60.0f });
	collider->SetEnable(false);
	SetOrigin(Origins::BC);
	isFishFired = false;
	if (seal != nullptr)
	{
		seal->SetActive(false);
	}
}

void IceHole::Update(float dt)
{
	time += dt * manager->GetSpeed();

	size = Utils::Lerp(sizeMin, sizeMax, time, false);
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));
	if (!isFishFired && seal == nullptr && position.y > 100.0f)
	{
		isFishFired = true;
		if (Utils::RandomRange(0, 2) == 1)
		{
			Fish* fish = manager->GetFish();
			fish->sortLayer = 30;
			fish->Reset();
			fish->SetPosition(this->GetPosition());
			fish->Fire();
			fish->SetActive(true);
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(fish);
		}
	}

	if (position.y > 160.0f)
	{
		collider->SetEnable(true);
	}
	SlicedSpriteGO::Update(dt);
	if (seal != nullptr)
	{
		seal->SetPosition({ position.x,  position.y - size.y * 0.5f });
	}

	if (FRAMEWORK.GetWindowSize().x + size.y < position.y)
	{
		manager->ReturnIceHole(this);
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
