#pragma once
#include "SpriteGO.h"
#include "Animator.h"
#include <AudioSource.h>

class Penta : public SpriteGO
{
protected:
	bool isJump = false;
	Animator* animator = nullptr;
	sf::Vector2f direction;
	float speed = 50.0f; 
	AudioSource* audio;
public:
	Penta(const std::string& textureID = "", const std::string& name = "")
		:SpriteGO(textureID, name) {}
	virtual ~Penta() override { Release(); };

	void SetAnimator(Animator* animator);


	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	void OnCollisionEnter(Collider* col);
};

