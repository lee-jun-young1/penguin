#include "stdafx.h"
#include "AnimatorPlayer.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>

void AnimatorPlayer::SetAnimator(Animator* animator)
{
	this->animator = animator;
}

void AnimatorPlayer::Init()
{
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleLeft.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleRight.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleBack.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleFront.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveLeft.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveRight.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveBack.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveFront.csv");

	//TODO : File Read To Add
	animator->LoadFromFile("animations/AnimatorPlayer");

	audio = new AudioSource(*this);
	AddComponent(audio);

	SetOrigin(Origins::BC);
}

void AnimatorPlayer::Reset()
{
	animator->SetState("IdleFront");
	animator->Play();
	SetOrigin(origin);
	SetPosition(0, 0);
	spriteDirection = { 1.0f, 0.0f };
	direction.y = 1.0f;
	audio->SetClip(RESOURCE_MANAGER.GetSoundBuffer("sound/powerup.wav"));
}

void AnimatorPlayer::Update(float dt)
{
	SpriteGO::Update(dt);

	
	Vector2f axis = { INPUT.GetAxis(Axis::Horizontal), INPUT.GetAxis(Axis::Vertical) };
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

	position += axis * dt * speed;
	SetPosition(position);

	if (Utils::SqrMagnitude(axis) == 0.0f)
	{
		//Idle
		animator->SetEvent("Idle");
	}
	else
	{
		if (abs(axis.x) > abs(axis.y))
		{
			if (axis.x < 0.0f)
			{
				animator->SetEvent("Left");
				sprite.setScale({ abs(sprite.getScale().x) * 1.0f, sprite.getScale().y });
			}
			else
			{
				animator->SetEvent("Right");
				sprite.setScale({ abs(sprite.getScale().x) * -1.0f, sprite.getScale().y });
			}
		}
		else
		{
			if (axis.y < 0.0f)
			{
				animator->SetEvent("Back");
				sprite.setScale({ abs(sprite.getScale().x) * 1.0f, sprite.getScale().y });
			}
			else
			{
				animator->SetEvent("Front");
				sprite.setScale({ abs(sprite.getScale().x) * 1.0f, sprite.getScale().y });
			}
		}
		//Move
		animator->SetEvent("Move");
	}
	if (INPUT.GetKeyDown(sf::Keyboard::F))
	{
		audio->Play();
	}
}

void AnimatorPlayer::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}
