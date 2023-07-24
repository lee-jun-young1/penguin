#pragma once
#include "SlicedSpriteGO.h"
#include <Seal.h>
#include "BoxCollider.h"
class StageManager;

class IceHole : public SlicedSpriteGO
{
protected:
	bool isFishFired = false;

	StageManager* manager;

	const sf::Vector2f sizeMin = { 27.0f, 0.0f };
	const sf::Vector2f sizeMax = { 40.0f, 14.0f };
	sf::Vector2f size;
	BoxCollider* collider;
	Seal* seal;

	sf::Vector2f startPos;
	sf::Vector2f endPos;

	float time = 0.0f;
public:
	IceHole(const std::string& textureID = "graphics/IceHole.png", sf::Rect<float> centerRect = { 10, 6, 20, 2 }, sf::Rect<float> size = { 0, 0, 40, 13 });
	void SetManager(StageManager* manager);

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	virtual void SetSize(sf::Vector2f size) override;

	void SetDirection(sf::Vector2f startPos, sf::Vector2f endPos);

	void SetSeal(Seal* seal);
	Seal* GetSeal();
};

