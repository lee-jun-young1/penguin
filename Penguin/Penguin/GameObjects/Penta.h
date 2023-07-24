#pragma once
#include "SpriteGO.h"
#include "Animator.h"
#include <AudioSource.h>

enum class State
{
	Move,
	Jump,
	InHole,
	Hit,
	Pegicopter,
};

class Penta : public SpriteGO
{
protected:
	SpriteGO* pegicopter = nullptr;
	Animator* pegicopterAni = nullptr;
	bool hasPegicopter = false;
	float pegicopterTime = 0.0f;
	float pegicopterDuration = 4.0f;
	const sf::Vector2f pegicopterOrigin = { 0.5f, 1.05f };

	State state = State::Move;
	Animator* animator = nullptr;
	sf::Vector2f direction;
	float speed = 50.0f; 

	AudioSource* audio;
	//TODO SoundClip
	sf::SoundBuffer* jumpSound;
	sf::SoundBuffer* HitHighSound;
	sf::SoundBuffer* HitLowSound;
	RigidBody2D* rigidBody;

	GameObject* crevasse;

	std::function<void(float)> updateFunc;
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
	void UpdateJump(float deltaTime);
	void UpdatePegicopter(float deltaTime);
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnCollisionEnter(Collider* col) override;

	void SetOrigin(Origins origin) override;
	void SetOrigin(float originX, float originY) override;

	void GetPegicopterItem();
	void SetPegicopter(SpriteGO* pegicopter, Animator* pegicopterAni);
};

