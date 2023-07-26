#include "stdafx.h"
#include "Fish.h"
#include "Utils.h"
#include "StageManager.h"
#include "BoxCollider.h"
#include <SceneGame.h>
#include <SceneManager.h>
#include <Penta.h>

void Fish::Fire()
{
	float random = Utils::RandomRange(-1.0f, 1.0f);
	rigidBody->SetGravity(true);
	rigidBody->SetVelocity({ random * 50.0f, -50.0f });

	animation->Play();
}

void Fish::Init()
{
	SpriteGO::Init();

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
	auto clip = Resources.GetAnimationClip(random ? "animations/Fish_Type2.csv" : "animations/Fish_Type1.csv" );
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
		manager->IncreaseScore(ScoreItemType::Fish);
		manager->ReturnFish(this);
		Penta& player = dynamic_cast<Penta&>(col->GetGameObject());
		player.PlayFishSound();
	}
}

void Fish::SetManager(StageManager* manager)
{
	this->manager = manager;
}