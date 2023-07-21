#include "stdafx.h"
#include "FlagItem.h"
#include "StageManager.h"

void FlagItem::Init()
{
	physicsLayer = 4;
	collider = new BoxCollider(*this);
	collider->SetTrigger(true);
	collider->SetRect({ 0.0f, 0.0f, 14.0f, 12.0f });
	AddComponent(collider);
}

void FlagItem::Reset()
{
	SpriteGO::Reset();
	collider->SetEnable(false);
}

void FlagItem::Update(float deltaTime)
{
	time += deltaTime * manager->GetSpeed();

	size = Utils::Lerp(sizeMin, sizeMax, time, false);
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));

	if (position.y > 160.0f)
	{
		collider->SetEnable(true);
	}

	SpriteGO::Update(deltaTime);
}

void FlagItem::OnTriggerEnter(Collider* col)
{
	if (col->GetGameObject().GetName() == "Player")
	{
		manager->IncreaseScore(100);
		manager->ReturnFlag(this);
	}
}

void FlagItem::SetManager(StageManager* manager)
{
	this->manager = manager;
}

void FlagItem::SetSize(sf::Vector2f size)
{
	sprite.setScale({ size.x / sizeMax.x, size.y / sizeMax.y });
	SetOrigin(Origins::BC);
	collider->SetRect({ position.x, position.y, size.x, size.y });
	collider->SetOffset({ size.x * -0.5f, -size.y });
}

void FlagItem::SetDirection(sf::Vector2f startPos, sf::Vector2f endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}