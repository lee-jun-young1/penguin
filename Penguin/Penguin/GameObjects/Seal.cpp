#include "stdafx.h"
#include "Seal.h"
#include "ObstacleManager.h"

Seal::Seal()
	:SpriteGO("graphics/Seal.png", "Seal")
{
}

void Seal::SetManager(ObstacleManager* manager)
{
	this->manager = manager;
}

void Seal::Init()
{
	SpriteGO::Init();
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Hide.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Ready.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Pop.csv");

	physicsLayer = 5;
	sortOrder = 2;

	Animator* sealAnimator = new Animator(*this);
	AddComponent(sealAnimator);
	SetAnimator(sealAnimator);

	collider = new BoxCollider(*this);
	collider->SetTrigger(true);
	AddComponent(collider);


	//TODO : File Read To Add
	animator->LoadFromFile("animations/Seal");
}

void Seal::Release()
{
	SpriteGO::Release();
}

void Seal::Reset()
{
	SpriteGO::Reset();
	time = 0;
	animator->SetState("Hide");
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
	if (time >= 1.0f)
	{
		collider->SetEnable(true);
	}
	else if (time >= 0.8f)
	{
		animator->SetEvent("Pop");
	}
	else if (time >= 0.4f)
	{
		animator->SetEvent("Ready");
	}
}

void Seal::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}

void Seal::OnTriggerEnter(Collider* col)
{
}

void Seal::OnTriggerStay(Collider* col)
{
}

void Seal::OnTriggerExit(Collider* col)
{
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