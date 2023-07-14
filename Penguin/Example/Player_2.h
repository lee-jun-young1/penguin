#pragma once
#include "SpriteGO.h"
#include "Animation.h"

class Player_2 : public SpriteGO
{
protected:
	bool isJump = false;
	AnimationController animation;
	sf::Vector2f direction;
	float speed = 100.0f;
public:
	Player_2(const std::string& textureID = "", const std::string& name = "")
		:SpriteGO(textureID, name) {}
	virtual ~Player_2() override { Release(); };


	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

