#pragma once
#include "SpriteGO.h"
#include <Animator.h>
#include <BoxCollider.h>
class Seal : public SpriteGO
{
protected:
	float time = 0;
	Animator* animator = nullptr;
	BoxCollider* collider = nullptr;
public:
	Seal();
	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;


	virtual void OnTriggerEnter(Collider* col) override;
	virtual void OnTriggerStay(Collider* col) override;
	virtual void OnTriggerExit(Collider* col) override;
	void SetAnimator(Animator* animator);

	virtual void SetPosition(const sf::Vector2f& position) override;
	virtual void SetPosition(const float& x, const float& y) override;

};

