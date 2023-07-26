#include "stdafx.h"
#include "IceStation.h"
#include "StageManager.h"
#include "Penta.h"

void IceStation::Init()
{
	SpriteGO::Init();
	flag = new SpriteGO("", "ClearFlag");
}

void IceStation::Reset()
{
	time = 0.0f;
	SpriteGO::Reset();
	size = sizeMax;
	SetSize(size);
	flagStartLocalPos = { 0.0f, -sprite.getGlobalBounds().height * 0.6f };
	flagEndLocalPos = { 0.0f, -sprite.getGlobalBounds().height * 1.0f };
	size = sizeMin;
	SetSize(size);
	SetPosition(startPos);
	flag->SetPosition(position + flagEndLocalPos);
	flag->SetActive(false);
}

void IceStation::Update(float deltaTime)
{
	
 	time += deltaTime * (time < 1.0f ? manager->GetSpeed() * 0.2f : 0.2f) ;

	size = Utils::Lerp(sizeMin, sizeMax, time);
	SetPosition(Utils::Lerp(startPos, endPos, time));
	SetSize(size); 

	if (time >= 1.0f)
	{
		flag->SetActive(true);
		flag->SetPosition(position + Utils::Lerp(flagStartLocalPos, flagEndLocalPos, time - 1.0f));
		flag->SetTexture(*Resources.GetTexture("graphics/" + manager->GetStageName() + ".png"));
	}

	SpriteGO::Update(deltaTime);
}

void IceStation::SetManager(StageManager* manager)
{
	this->manager = manager;
}

void IceStation::SetSize(sf::Vector2f size)
{
	sprite.setScale(size);
	SetOrigin(Origins::BC);
}

void IceStation::SetDirection(sf::Vector2f startPos, sf::Vector2f endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}

void IceStation::Draw(sf::RenderWindow& window)
{
	if (flag->IsActive())
	{
		flag->Draw(window);
	}
	SpriteGO::Draw(window);
}
