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
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f, 60.0f });
}

void IceHole::Update(float dt)
{
	size.x += dt;
	size.y += dt;
	SetSize(size);
	position.y += dt * 10.0f;
	SetPosition(position);
}
