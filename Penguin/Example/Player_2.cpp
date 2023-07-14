#include "stdafx.h"
#include "Player_2.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>

void Player_2::Init()
{
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleLeft.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleRight.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleBack.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/IdleFront.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveLeft.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveRight.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveBack.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/MoveFront.csv");


	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/IdleLeft.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/IdleRight.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/IdleBack.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/IdleFront.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/MoveLeft.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/MoveRight.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/MoveBack.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/MoveFront.csv"));

	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);
}

void Player_2::Reset()
{
	animation.Play("IdleFront");
	SetOrigin(origin);
	SetPosition(0, 0);
	spriteDirection = { 1.0f, 0.0f };
	direction.y = 1.0f;
}

void Player_2::Update(float dt)
{

	
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
		if (abs(direction.x) > abs(direction.y))
		{
			if (direction.x < 0.0f && animation.GetCurrentClipName() != "IdleLeft")
			{
				animation.Play("IdleLeft");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
			if (direction.x > 0.0f && animation.GetCurrentClipName() != "IdleRight")
			{
				animation.Play("IdleRight");
				sprite.setScale(abs(sprite.getScale().x) * -1.0f, sprite.getScale().y);
			}
		}
		else
		{
			if (direction.y < 0.0f && animation.GetCurrentClipName() != "IdleBack")
			{
				animation.Play("IdleBack");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
			if (direction.y > 0.0f && animation.GetCurrentClipName() != "IdleFront")
			{
				animation.Play("IdleFront");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
		}
	}
	else
	{
		if (abs(axis.x) > abs(axis.y))
		{
			if (axis.x < 0.0f && animation.GetCurrentClipName() != "MoveLeft")
			{
				animation.Play("MoveLeft");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
			if (axis.x > 0.0f && animation.GetCurrentClipName() != "MoveRight")
			{
				animation.Play("MoveRight");
				sprite.setScale(abs(sprite.getScale().x) * -1.0f, sprite.getScale().y);
			}
		}
		else
		{
			if (axis.y < 0.0f && animation.GetCurrentClipName() != "MoveBack")
			{
				animation.Play("MoveBack");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
			if (axis.y > 0.0f && animation.GetCurrentClipName() != "MoveFront")
			{
				animation.Play("MoveFront");
				sprite.setScale(abs(sprite.getScale().x) * 1.0f, sprite.getScale().y);
			}
		}
	}
	


	animation.Update(dt);
}

void Player_2::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}
