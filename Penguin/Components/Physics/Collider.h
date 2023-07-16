#pragma once
#include "Component.h"
class RigidBody2D;

class Collider : public Component
{
protected:
	RigidBody2D* rigidbody;
	ColliderType type;
	float bounciness;
	std::list<Collider*> prevCollideList;
	bool isCollide;

	bool isTrigger;

	sf::Vector2f offset;

	void OnCollisionEnter(Collider* col);
	void OnCollisionStay(Collider* col);
	void OnCollisionExit(Collider* col);

	void OnTriggerEnter();
	void OnTriggerStay();
	void OnTriggerExit(Collider* col);
public:
	Collider(GameObject& gameObject, ColliderType type);
	virtual ~Collider();

	ColliderType GetColliderType();

	RigidBody2D* GetRigidBody();
	void SetRigidbody(RigidBody2D* rigidbody);
	void SetBounciness(float bounciness);

	virtual sf::Vector2f GetNormal(Collider* col);
	virtual bool CheckCross(Collider* col) = 0;
	virtual sf::Vector2f GetCenter() = 0;
	virtual float GetWidth() = 0;
	virtual float GetHeight() = 0;

	void SetTrigger(bool isTrigger);

	void SetOffset(sf::Vector2f offset);

	sf::Vector2f GetOffset();

	// Collider��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	bool CheckAllCollide(std::list<Collider*>& colliderList);
};