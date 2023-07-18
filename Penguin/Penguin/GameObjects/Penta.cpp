#include "stdafx.h"
#include "Penta.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>
#include "AnimationClip.h"

void Penta::SetAnimator(Animator* animator)
{
	this->animator = animator;
}

void Penta::Init()
{
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_Jump.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_Move.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_Hit.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_CrevasseIdle.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_CrevasseMove.csv");

	//TODO : File Read To Add
	animator->LoadFromFile("animations/Penta");


	audio = new AudioSource(*this);
	AddComponent(audio);

	SetOrigin(Origins::BC);
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ position.x, position.y, 30.0f, 3.0f });
	boxCol->SetOffset({ 30.0f * -0.5f, -3.0f});
}

void Penta::Reset()
{
	animator->SetState("Move");
	animator->Play();
	SetOrigin(origin);
	SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, 150.0f);
	spriteDirection = { 1.0f, 0.0f };
	direction.y = 1.0f;
	jumpSound = RESOURCE_MANAGER.GetSoundBuffer("sound/sfx/6_Jump.wav");
	HitHighSound = RESOURCE_MANAGER.GetSoundBuffer("sound/sfx/2_Collided_High.wav");
	HitLowSound = RESOURCE_MANAGER.GetSoundBuffer("sound/sfx/3_Collided_Low.wav");

#pragma region AnimationAction
	AnimationClip& hitClip = animator->GetState("Hit")->clip;

	hitClip.frames[0].action =
		[this]()
	{
		if (!isJump)
		{
			rigidBody->AddForce({ 0.0f, -40.0f });
		}
		audio->SetClip(HitHighSound);
		audio->Play();
	};
	hitClip.frames[1].action =
		[this]()
	{
		rigidBody->AddForce({ 0.0f, -40.0f });
		audio->SetClip(HitLowSound);
		audio->Play();
	};
	hitClip.frames[2].action =
		[this]()
	{
		rigidBody->AddForce({ 0.0f, -40.0f });
		audio->SetClip(HitLowSound);
		audio->Play();
	};
	hitClip.frames[3].action =
		[this]()
	{
		animator->SetEvent("HitEnd");
		SetFlipX(false);
		stateUpdate = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
	};

	AnimationClip& CrevasseIdle = animator->GetState("CrevasseIdle")->clip;
	CrevasseIdle.frames[4].action =
		[this]()
	{
		animator->SetEvent("Move");
	};
	AnimationClip& CrevasseMove = animator->GetState("CrevasseMove")->clip;
	CrevasseMove.frames[11].action =
		[this]()
	{
		animator->SetEvent("Idle");
	};

#pragma endregion

	stateUpdate = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
}

void Penta::Update(float dt)
{
	SpriteGO::Update(dt);


	if (INPUT.GetKeyDown(sf::Keyboard::Num2))
	{
		animator->SetEvent("Hit");
	}

	stateUpdate(dt);
}

void Penta::UpdateHit(float deltaTime)
{
	rigidBody->SetVelocity({ direction.x * speed * 0.5f, rigidBody->GetVelocity().y });
}
void Penta::UpdateCrevasse(float deltaTime)
{
	if (INPUT.GetKeyDown(sf::Keyboard::Space) && animator->GetClipName() == "CrevasseMove")
	{
		animator->SetEvent("Escape");
		stateUpdate = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
	}
}
void Penta::UpdateMove(float deltaTime)
{
	if (!isJump && INPUT.GetKeyDown(sf::Keyboard::Space))
	{
		//Jump
		rigidBody->AddForce({ 0.0f, -100.0f });
		animator->SetEvent("Jump");
		audio->SetClip(jumpSound);
		audio->Play();
		isJump = true;
	}
	Vector2f axis = { INPUT.GetAxis(Axis::Horizontal), 0.0f };

	if (Utils::SqrMagnitude(axis) != 0.0f)
	{
		if (abs(axis.x) > abs(axis.y))
		{
			direction = { axis.x < 0.0f ? -1.0f : 1.0f, 0.0f };
		}
		else
		{
			direction = { 0.0f, axis.y < 0.0f ? -1.0f : 1.0f };
		}
	}
	rigidBody->SetVelocity({ axis.x * speed , rigidBody->GetVelocity().y });
}

void Penta::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}

void Penta::OnCollisionEnter(Collider* col)
{
	if (col->GetGameObject().GetName() == "Seal")
	{
		col->GetGameObject().GetName();
	}
	if (col->GetGameObject().GetName() == "Ground")
	{
		isJump = false;
		animator->SetEvent("Move");
	}

	if (col->GetGameObject().GetName() == "IceHole" || col->GetGameObject().GetName() == "Seal" || col->GetGameObject().GetName() == "CrevasseSide")
	{
		animator->SetEvent("Hit");
		if (position.x < col->GetGameObject().GetPosition().x)
		{
			direction.x = -1.0f;
			SetFlipX(false);
		}
		else
		{
			direction.x = 1.0f;
			SetFlipX(true);
		}
		stateUpdate = std::bind(&Penta::UpdateHit, this, std::placeholders::_1);

	}
	if (col->GetGameObject().GetName() == "CrevasseCenter")
	{
		animator->SetEvent("Crevasse");
		stateUpdate = std::bind(&Penta::UpdateCrevasse, this, std::placeholders::_1);
	}
}