#pragma once
#include "SlicedSpriteGO.h"
#include <Seal.h>
#include "BoxCollider.h"
class IceHole : public SlicedSpriteGO
{
protected:
	sf::Vector2f size;
	BoxCollider* collider;
	Seal* seal;
public:
	IceHole(const std::string& textureID = "graphics/IceHole.png", sf::Rect<float> centerRect = { 10, 6, 20, 2 }, sf::Rect<float> size = { 0, 0, 40, 13 });
	virtual void OnTriggerEnter(Collider* col) override;
	virtual void OnTriggerStay(Collider* col) override;
	virtual void OnTriggerExit(Collider* col) override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	virtual void SetSize(sf::Vector2f size) override;

	void SetSeal(Seal* seal);
};

