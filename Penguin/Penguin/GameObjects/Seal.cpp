#include "stdafx.h"
#include "Seal.h"
#include "StageManager.h"

Seal::Seal()
	:SpriteGO("graphics/Seal.png", "Seal")
{
}

void Seal::SetManager(StageManager* manager)
{
	this->manager = manager;
}

void Seal::Init()
{
	SpriteGO::Init();
	Resources.Load(ResourceTypes::AnimationClip, "animations/Seal_Hide.csv");
	Resources.Load(ResourceTypes::AnimationClip, "animations/Seal_Ready.csv");
	Resources.Load(ResourceTypes::AnimationClip, "animations/Seal_Pop.csv");

	physicsLayer = 5;
	sortOrder = 2;

	animator = new Animator(*this, "animations/Seal", "Hide");
	AddComponent(animator);

	collider = new BoxCollider(*this);
	collider->SetTrigger(true);
	AddComponent(collider);
}

void Seal::Release()
{
	SpriteGO::Release();
}

void Seal::Reset()
{
	SpriteGO::Reset();
	time = 0;
	animator->Play();
	SetOrigin(Origins::BC);

	collider->SetEnable(false);
	collider->SetRect({ position.x, position.y, GetSize().x, GetSize().y });
	collider->SetOffset({ GetSize().x * -0.5f , -GetSize().y });
}

void Seal::Update(float dt)
{
	SpriteGO::Update(dt);
	time += dt * manager->GetSpeed();
	if (time >= 0.8f)
	{
		animator->SetEvent("Pop");
		collider->SetEnable(true);
	}
	else if (time >= 0.6f)
	{
		animator->SetEvent("Ready");
	}
}

void Seal::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}


void Seal::SetAnimator(Animator* animator)
{
	this->animator = animator;
}

void Seal::SetPosition(const sf::Vector2f& position)
{
	this->position = position;
	if (this->animator->GetClipName() == "Pop")
	{
		this->position.y += 3.0f;
	}
	sprite.setPosition(this->position);
}

void Seal::SetPosition(const float& x, const float& y)
{
	this->position.x = x;
	this->position.y = y;
	if (this->animator->GetClipName() == "Pop")
	{
		this->position.y += 3.0f;
	}
	sprite.setPosition(this->position);
}