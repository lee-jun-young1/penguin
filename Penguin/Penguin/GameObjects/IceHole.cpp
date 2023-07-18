#include "stdafx.h"
#include "IceHole.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Penta.h>

IceHole::IceHole(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size, "IceHole")
{
}

void IceHole::OnTriggerEnter(Collider* col)
{
}

void IceHole::OnTriggerStay(Collider* col)
{
}

void IceHole::OnTriggerExit(Collider* col)
{
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
	size = { 27.0f, 0.0f };
	SetSize(size);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.4f, 60.0f });
	collider->SetEnable(false);
}

void IceHole::Update(float dt)
{
	SlicedSpriteGO::Update(dt);
	size.x += dt;
	size.y += dt;
	SetSize(size);
	position.y += dt * 20.0f;
	SetPosition(position);
	if (position.y > 160.0f)
	{
		collider->SetEnable(true);
	}
	if (seal != nullptr)
	{
		seal->SetPosition({ position.x + size.x * 0.5f, position.y + size.y * 0.5f});
	}
}

void IceHole::SetSize(sf::Vector2f size)
{
	SlicedSpriteGO::SetSize(size);
	collider->SetRect({ position.x, position.y, size.x, size.y });
}

void IceHole::SetSeal(Seal* seal)
{
	this->seal = seal;
}
