#pragma once
#include "SpriteGO.h"
#include <Animator.h>
#include <BoxCollider.h>

class StageManager;

class Seal : public SpriteGO
{
protected:
	StageManager* manager;

	float time = 0;
	Animator* animator = nullptr;
	BoxCollider* collider = nullptr;
public:
	Seal();
	void SetManager(StageManager* manager);
	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;


	void SetAnimator(Animator* animator);

	virtual void SetPosition(const sf::Vector2f& position) override;
	virtual void SetPosition(const float& x, const float& y) override;

};

