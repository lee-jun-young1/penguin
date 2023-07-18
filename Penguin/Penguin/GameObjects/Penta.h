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
	//TODO SoundClip
	sf::SoundBuffer* jumpSound;
	sf::SoundBuffer* HitHighSound;
	sf::SoundBuffer* HitLowSound;
	RigidBody2D* rigidBody;

	std::function<void(float)> stateUpdate;
public:
	Penta(const std::string& textureID = "", const std::string& name = "")
		:SpriteGO(textureID, name) {}
	virtual ~Penta() override { Release(); };

	void SetAnimator(Animator* animator);
	void SetRigidBody(RigidBody2D* rigidBody) { this->rigidBody = rigidBody; }


	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	void UpdateHit(float deltaTime);
	void UpdateCrevasse(float deltaTime);
	void UpdateMove(float deltaTime);
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnCollisionEnter(Collider* col) override;
};

