#include "stdafx.h"
#include "IceHole.h"
#include <Framework.h>

IceHole::IceHole(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size)
{

}

void IceHole::OnTriggerEnter(Collider* col)
{
	//충돌처리
}

void IceHole::OnTriggerStay(Collider* col)
{
}

void IceHole::OnTriggerExit(Collider* col)
{
}

void IceHole::Reset()
{
	SlicedSpriteGO::Reset();
	size = { 27.0f, 0.0f };
	SetSize(size);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.4f, 60.0f });
}

void IceHole::Update(float dt)
{
	size.x += dt;
	size.y += dt;
	SetSize(size);
	position.y += dt * 20.0f;
	SetPosition(position);
	if (seal != nullptr)
	{
		seal->SetPosition({ position.x + size.x * 0.5f, position.y + size.y * 0.5f});
	}
}

void IceHole::SetSeal(Seal* seal)
{
	this->seal = seal;
}
