#pragma once
#include "SpriteGO.h"
#include "Animator.h"
#include <AudioSource.h>
#include <Framework.h>

enum class PentaState
{
	Move,
	Jump,
	InHole,
	Hit,
	Pegicopter,
	Clear,
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

	PentaState state = PentaState::Move;
	Animator* animator = nullptr;
	sf::Vector2f direction;
	float speed = 50.0f; 

	AudioSource* audio;
	//TODO SoundClip
	sf::SoundBuffer* fishSound;
	sf::SoundBuffer* flagSound;
	sf::SoundBuffer* jumpSound;
	sf::SoundBuffer* hitHighSound;
	sf::SoundBuffer* hitLowSound;
	sf::SoundBuffer* crevasseSound;
	sf::SoundBuffer* pegicopterSound;
	RigidBody2D* rigidBody;

	GameObject* crevasse;

	GameObject* shadow;

	std::function<void(float)> updateFunc;

	float afterClearTime;
	sf::Vector2f clearPosition;
	sf::Vector2f clearTargetPosition = { FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().x * 0.5f };
	/// <summary>
	/// 원심력 방향
	/// </summary>
	float centrifugalForceDirection = 0.0f;

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

	virtual void UpdateHit(float deltaTime);
	virtual void UpdateCrevasse(float deltaTime);
	void UpdateIdle(float deltaTime);
	virtual void UpdateMove(float deltaTime);
	virtual void UpdateClear(float deltaTime);
	virtual void UpdateJump(float deltaTime);
	virtual void UpdatePegicopter(float deltaTime);

	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnCollisionEnter(Collider* col) override;

	void SetOrigin(Origins origin) override;
	void SetOrigin(float originX, float originY) override;

	void GetPegicopterItem();
	void SetPegicopter(SpriteGO* pegicopter, Animator* pegicopterAni);

	void SetShadow(GameObject* shadow) { this->shadow = shadow; };
	void Clear();

	void PlayFlagSound();

	void PlayFishSound();

	virtual void StartStage();

	const PentaState& GetState() { return state; }

	// 원심력 방향 설정
	void SetCentrifugalForceDirection(const float& force) { centrifugalForceDirection = force; }
};

