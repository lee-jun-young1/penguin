#pragma once
#include "SpriteGO.h"
#include <RectangleShapeGO.h>
#include <Animator.h>

enum class Load
{
	Left,
	Right,
	Straight,
};

class Background : public SpriteGO
{
	RectangleShapeGO* sky;
	Animator* animator;

	float loadRefreshCycle = 5.0f;
	float loadRefreshTime = 0.0f;
public:
	Background(const std::string textureID = "", const std::string& name = "");

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetSkyColor(const sf::Color& color);
	void SetLoad(Load load);
	void SetSpeed(const float& speed) { animator->SetSpeed(speed); };
};

