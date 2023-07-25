#include "stdafx.h"
#include "FlagItem.h"
#include "StageManager.h"
#include "Penta.h"
#include <SceneManager.h>
#include <Penguin/Scenes/SceneGame.h>

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
	time = 0;
	SpriteGO::Reset();
	collider->SetEnable(false);
	if (Utils::RandomValue() < manager->GetFlagItemPercentage())
	{
		itemType = ScoreItemType::PegicopterFlag;
	}
	else
	{
		itemType = ScoreItemType::Flag;
		sprite.setColor(sf::Color::Green);
	}
}

void FlagItem::Update(float deltaTime)
{
	time += deltaTime * manager->GetSpeed();

	size = Utils::Lerp(sizeMin, sizeMax, time, false);
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));

	if (time > 0.85f)
	{
		collider->SetEnable(true);
	}

	if (itemType == ScoreItemType::PegicopterFlag)
	{
		int t = time * 100;
		sf::Color color;
		color.r = Utils::Lerp(0, 255, (t % 100) * 0.01f);
		color.g = Utils::Lerp(0, 255, ((t + 33) % 100) * 0.01f);
		color.b = Utils::Lerp(0, 255, ((t + 66) % 100) * 0.01f);
		sprite.setColor(color);
	}

	if (FRAMEWORK.GetWindowSize().x + sprite.getGlobalBounds().height < position.y)
	{
		manager->ReturnFlag(this);
		//manager->ReturnFlag(this);
	}

	SpriteGO::Update(deltaTime);
}

void FlagItem::OnTriggerEnter(Collider* col)
{
	if (col->GetGameObject().GetName() == "Player")
	{
		Penta& player = dynamic_cast<Penta&>(col->GetGameObject());
		manager->IncreaseScore(itemType);
		player.PlayFlagSound();
		manager->ReturnFlag(this);
		if (itemType == ScoreItemType::PegicopterFlag)
		{
			player.GetPegicopterItem();
		}
	}
}

void FlagItem::OnTriggerStay(Collider* col)
{
	//if (col->GetGameObject().GetName() == "Player")
	//{
	//	manager->ReturnFlag(this);
	//	manager->IncreaseScore(itemType);
	//	Penta& player = dynamic_cast<Penta&>(col->GetGameObject());
	//	player.PlayFlagSound();
	//	if (itemType == ScoreItemType::PegicopterFlag)
	//	{
	//		player.GetPegicopterItem();
	//	}
	//}
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