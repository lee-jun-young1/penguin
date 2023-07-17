#include "stdafx.h"
#include "Seal.h"

Seal::Seal()
	:SpriteGO("graphics/Seal.png", "Seal")
{
}

void Seal::Init()
{
	SpriteGO::Init();
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Hide.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Ready.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Seal_Pop.csv");

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
}

void Seal::Update(float dt)
{
	time += dt;
	int intTime = time;
	if (intTime >= 2)
	{
		animator->SetEvent("Pop");
	}
	else if (intTime >= 1)
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
