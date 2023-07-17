#pragma once
#include "SlicedSpriteGO.h"
#include <SpriteGO.h>
class IceHole : public SlicedSpriteGO
{
protected:
	sf::Vector2f size;
	SpriteGO* seal;
public:
	IceHole(const std::string& textureID = "graphics/IceHole.png", sf::Rect<float> centerRect = { 10, 6, 20, 2 }, sf::Rect<float> size = { 0, 0, 40, 13 });
	virtual void OnTriggerEnter(Collider* col) override;
	virtual void OnTriggerStay(Collider* col) override;
	virtual void OnTriggerExit(Collider* col) override;

	virtual void Reset() override;
	virtual void Update(float dt) override;
};

