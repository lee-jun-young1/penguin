#pragma once
#include "Component.h"
#include <GameObject.h>
class RigidBody2D : public Component
{
protected:
	bool useGravity;
	bool isCollided;
	sf::Vector2f velocity;
public:
	RigidBody2D(GameObject& gameObject);
	// Component을(를) 통해 상속됨

	void SetGravity(bool useGravity);

	sf::Vector2f GetVelocity();
	void AddForce(sf::Vector2f velocity);
	void SetVelocity(sf::Vector2f velocity);

	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;
	void OnCollisionEnter(Collider* thisCol, Collider* diffCol);
	void OnCollisionStay(Collider* thisCol, Collider* diffCol);
	void OnCollisionExit(sf::Vector2f normal);

	void OnTriggerEnter();
	void OnTriggerStay();
	void OnTriggerExit(sf::Vector2f normal);
};