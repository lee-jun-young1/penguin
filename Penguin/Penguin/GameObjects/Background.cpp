#include "stdafx.h"
#include "Background.h"
#include <SceneManager.h>
#include <Penta.h>
#include <InputManager.h>
#include "Utils.h"

Background::Background(const std::string textureID, const std::string& name)
	:SpriteGO(textureID, name)
{
}

void Background::Init()
{
	sky = new RectangleShapeGO("Sky");
	sky->SetFillColor({ 96, 208, 224 });
	animator = new Animator(*this, "animations/Background", "Straight");
	AddComponent(animator);
	SpriteGO::Init();
}

void Background::Release()
{
	SpriteGO::Release();
}

void Background::Reset()
{
	SpriteGO::Reset();
	sky->SetSize(GetSize());
	sky->SetPosition(GetPosition());
	loadRefreshTime = 0.0f;
}

void Background::Update(float dt)
{
	SpriteGO::Update(dt);
	if (Input.GetKeyDown(sf::Keyboard::Insert))
	{
		SetLoad(Load::Left);
	}
	if (Input.GetKeyDown(sf::Keyboard::Home))
	{
		SetLoad(Load::Straight);
	}
	if (Input.GetKeyDown(sf::Keyboard::PageUp))
	{
		SetLoad(Load::Right);
	}
	loadRefreshTime += dt;
	if (loadRefreshTime > loadRefreshCycle)
	{
		SetLoad((Load)Utils::Clamp(Utils::RandomRange(1, 5), 1, 3));
		loadRefreshTime -= loadRefreshCycle;
	}
}

void Background::Draw(sf::RenderWindow& window)
{
	sky->Draw(window);
	SpriteGO::Draw(window);
}

void Background::SetSkyColor(const sf::Color& color)
{
	sky->SetFillColor(color);
}

void Background::SetLoad(Load load)
{
	switch (load)
	{
	case Load::Left:
		animator->SetEvent("Left");
		((Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"))->SetCentrifugalForceDirection(1.0f);
		break;
	case Load::Right:
		animator->SetEvent("Right");
		((Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"))->SetCentrifugalForceDirection(-1.0f);
		break;
	case Load::Straight:
		animator->SetEvent("Straight");
		((Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player"))->SetCentrifugalForceDirection(0.0f);
		break;
	}
}
