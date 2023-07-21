#pragma once
#include "SpriteGO.h"
#include "RigidBody2D.h"
#include "Animator.h"
class StageManager;
class BoxCollider;

class Fish : public SpriteGO
{
protected:
	StageManager* manager;

	Animation* animation;
	RigidBody2D* rigidBody;
	BoxCollider* collider;
public:
	Fish(const std::string textureID = "graphics/Fish.png", const std::string& name = "Fish") : SpriteGO(textureID, name) {}
	void Fire();

	virtual void Init() override;
	virtual void Reset() override; //�ʱ�ȭ
	virtual void Update(float deltaTime) override; //�ʱ�ȭ
	virtual void Draw(sf::RenderWindow& window) override; //�ʱ�ȭ

	virtual void OnTriggerEnter(Collider* col) override;
	void SetManager(StageManager* manager);
};

