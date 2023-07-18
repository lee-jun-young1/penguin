#include "stdafx.h"
#include "Crevasse.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>

Crevasse::Crevasse(const std::string& textureID, sf::Rect<float> centerRect, sf::Rect<float> size)
	:SlicedSpriteGO(textureID, centerRect, size)
{
}

void Crevasse::Init()
{
	center = new GameObject("CrevasseCenter");
	center->physicsLayer = 5;
	BoxCollider* centerCol = new BoxCollider(*center);
	centerCol->SetTrigger(true);
	centerCol->SetRect({ 20.0f, 0.0f, 40.0f, 8.0f });
	centerCol->SetOffset({ 30.0f, 0.0f });
	center->AddComponent(centerCol);

	side = new GameObject("CrevasseSide");
	side->physicsLayer = 5;
	BoxCollider* leftCol = new BoxCollider(*side);
	leftCol->SetTrigger(true);
	leftCol->SetRect({ 0.0f, 0.0f, 10.0f, 8.0f });
	BoxCollider* rightCol = new BoxCollider(*side);
	rightCol->SetTrigger(true);
	rightCol->SetRect({ 90.0f, 0.0f, 10.0f, 8.0f });
	rightCol->SetOffset({ 80.0f, 0.0f });
	side->AddComponent(leftCol);
	side->AddComponent(rightCol);
}

void Crevasse::Release()
{
	delete center;
	delete side;
}

void Crevasse::Reset()
{
	SlicedSpriteGO::Reset();
	size = { 60.0f, 0.0f };
	SetSize(size);
	SetOrigin(Origins::BL);
	SetPosition({ FRAMEWORK.GetWindowSize().x * 0.6f, 60.0f });
	time = 0.0f;
}

void Crevasse::SetDirection(sf::Vector2f startPos, sf::Vector2f endPos)
{
	this->startPos = startPos;
	this->endPos = endPos;
}

void Crevasse::Update(float dt)
{
	time += dt * speed;

	size.x += dt * 5.0f;
	size.y += dt;
	SetSize(size);
	SetPosition(Utils::Lerp(startPos, endPos, time, false));
	
	if (time > 0.0f)
	{
		center->SetPosition(position);
		side->SetPosition(position);
		center->SetActive(true);
		side->SetActive(true);
		center->Update(dt);
		side->Update(dt);
	}
}

void Crevasse::OnGUI(sf::RenderWindow& window)
{
#ifdef _DEBUG
	center->OnGUI(window);
	side->OnGUI(window);
#endif
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
