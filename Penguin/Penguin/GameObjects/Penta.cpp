#include "stdafx.h"
#include "Penta.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>

void Penta::SetAnimator(Animator* animator)
{
	this->animator = animator;
}

void Penta::Init()
{
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_Jump.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Penta_Move.csv");

	//TODO : File Read To Add
	animator->LoadFromFile("animations/Penta");

	audio = new AudioSource(*this);
	AddComponent(audio);

	SetOrigin(Origins::BC);
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ position.x, position.y, 32, 34 });
	boxCol->SetOffset({ 32.0f * -0.5f, -34.0f});
}

void Penta::Reset()
{
	animator->SetState("Move");
	animator->Play();
	SetOrigin(origin);
	SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, 150.0f);
	spriteDirection = { 1.0f, 0.0f };
	direction.y = 1.0f;
	audio->SetClip(RESOURCE_MANAGER.GetSoundBuffer("sound/sfx/6_Jump.wav"));
}

void Penta::Update(float dt)
{
	SpriteGO::Update(dt);

	
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

	if (!isJump && INPUT.GetKeyDown(sf::Keyboard::Space))
	{
		//Jump
		((RigidBody2D*)GetComponent(ComponentType::RigidBody))->AddForce({ 0.0f, -100.0f });
		animator->SetEvent("Jump");
		audio->Play();
		isJump = true;
	}

	position += axis * dt * speed;
	SetPosition(position);

	//if (Utils::SqrMagnitude(axis) == 0.0f)
	//{
	//	//Idle
	//	animator->SetEvent("Move");
	//}
}

void Penta::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}

void Penta::OnCollisionEnter(Collider* col)
{
	isJump = false;
	if (col->GetGameObject().GetName() == "Ground")
	{
		animator->SetEvent("Move");
	}
}