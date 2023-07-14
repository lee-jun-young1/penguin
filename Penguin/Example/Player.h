#pragma once
#include "SpriteGO.h"
#include "AnimationController.h"

class Player : public SpriteGO
{
protected:
	bool isJump = false;
	AnimationController animation;
	sf::Vector2f direction;
public:
	Player(const std::string& textureID = "", const std::string& name = "")
		:SpriteGO(textureID, name) {}
	virtual ~Player() override { Release(); };


	// GameObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void OnCollisionEnter(Collider* col) override;
};

