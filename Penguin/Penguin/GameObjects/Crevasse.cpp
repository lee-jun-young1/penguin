#include "stdafx.h"
#include "Crevasse.h"
#include <Framework.h>

Crevasse::Crevasse(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size)
{
}

void Crevasse::Init()
{
}

void Crevasse::Release()
{
}

void Crevasse::Reset()
{
	SlicedSpriteGO::Reset();
	size = { 60.0f, 0.0f };
	SetSize(size);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.6f, 60.0f });
}

void Crevasse::Update(float dt)
{
	size.x += dt * 5.0f;
	size.y += dt;
	SetSize(size);
	position.y += dt * 20.0f;
	SetPosition(position);
}

void Crevasse::Draw(sf::RenderWindow& window)
{
	SlicedSpriteGO::Draw(window);
}

void Crevasse::OnTriggerEnter(Collider* col)
{
}

void Crevasse::OnTriggerStay(Collider* col)
{
}

void Crevasse::OnTriggerExit(Collider* col)
{
}
