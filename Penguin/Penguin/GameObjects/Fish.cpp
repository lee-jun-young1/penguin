#include "stdafx.h"
#include "Fish.h"
#include "Utils.h"
#include "StageManager.h"
#include "BoxCollider.h"
#include <SceneGame.h>
#include <SceneManager.h>

void Fish::Fire()
{
	float random = Utils::RandomRange(-1.0f, 1.0f);
	rigidBody->SetGravity(true);
	rigidBody->SetVelocity({ random * 50.0f, -50.0f });

	animation->Play();
	if (random < 0.0f)
	{
		SetFlipX(true);
	}
	else
	{
		SetFlipX(false);
		collider->SetOffset({ -14.0f , 0.0f});
	}
}

void Fish::Init()
{
	SpriteGO::Init();
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Fish_Type1.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Fish_Type2.csv");

	physicsLayer = 4;
	animation = new Animation(*this);
	AddComponent(animation);
	collider = new BoxCollider(*this);
	collider->SetTrigger(true);
	collider->SetRect({ 0.0f, 0.0f, 14.0f, 12.0f });
	AddComponent(collider);
	rigidBody = new RigidBody2D(*this);
	AddComponent(rigidBody);
}

void Fish::Reset()
{
	SpriteGO::Reset();
	rigidBody->SetGravity(false);
	rigidBody->SetVelocity({ 0.0f, 0.0f });
	collider->SetEnable(false);
	bool random = Utils::RandomRange(0, 2) == 0;
	auto clip = RESOURCE_MANAGER.GetAnimationClip(random ? "animations/Fish_Type1.csv" : "animations/Fish_Type2.csv");
	animation->SetClip(clip);
}

void Fish::Update(float deltaTime)
{
	SpriteGO::Update(deltaTime);
	if (!collider->IsEnable() && rigidBody->GetVelocity().y > 0.0f)
	{
		collider->SetEnable(true);
	}
	if (FRAMEWORK.GetWindowSize().x + sprite.getGlobalBounds().height < position.y)
	{
		manager->ReturnFish(this);
	}
	//cout << position.x <<", " << position.y << endl;
}

void Fish::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}

void Fish::OnTriggerEnter(Collider* col)
{
	if (col->GetGameObject().GetName() == "Player")
	{
		manager->IncreaseScore(100);
		manager->ReturnFish(this);
	}
}

void Fish::SetManager(StageManager* manager)
{
	this->manager = manager;
}