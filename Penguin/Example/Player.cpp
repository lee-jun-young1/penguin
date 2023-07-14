#include "stdafx.h"
#include "Player.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include "ResourceManager.h"

void Player::Init()
{
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Idle.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Move.csv");
	RESOURCE_MANAGER.Load(ResourceTypes::AnimationClip, "animations/Jump.csv");


	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/Idle.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/Move.csv"));
	animation.AddClip(*RESOURCE_MANAGER.GetAnimationClip("animations/Jump.csv"));


	animation.SetTarget(&sprite);

	SetOrigin(Origins::BC);
}

void Player::Reset()
{
	animation.Play("Idle");
	SetOrigin(origin);
	SetPosition(0, 0);
	direction.x = 1.0f;
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ GetPosition().x, GetPosition().y, GetSize().x, GetSize().y });
	boxCol->SetOffset({ GetSize().x * 0.5f, GetSize().y * -1.0f });
}

void Player::Update(float dt)
{
	auto rig = (RigidBody2D*)GetComponent(ComponentType::RigidBody);
	rig->SetVelocity({ INPUT.GetAxis(Axis::Horizontal) * 100.0f, rig->GetVelocity().y });

	if (!isJump && INPUT.GetKeyDown(sf::Keyboard::Space))
	{
		isJump = true;
		animation.Play("Jump");
		rig->AddForce({ 0.0f, -1.0f * FRAMEWORK.GetDPM() * FRAMEWORK.GetGravity() });
	}
	
	if (INPUT.GetAxisRaw(Axis::Horizontal) != 0.0f)
	{
		direction.x = INPUT.GetAxisRaw(Axis::Horizontal);
	}

	sprite.setScale(direction.x * abs(sprite.getScale().x), abs(sprite.getScale().y));

	if (!isJump && (INPUT.GetKeyDown(sf::Keyboard::A) || INPUT.GetKeyDown(sf::Keyboard::D) ||
		INPUT.GetKeyDown(sf::Keyboard::Left) || INPUT.GetKeyDown(sf::Keyboard::Right)))
	{
		animation.Play("Move");
	}

	if (!isJump && (INPUT.GetKeyUp(sf::Keyboard::A) || INPUT.GetKeyUp(sf::Keyboard::D) ||
		INPUT.GetKeyUp(sf::Keyboard::Left) || INPUT.GetKeyUp(sf::Keyboard::Right)))
	{
		animation.Play("Idle");
	}


	
	//SpriteGO::Update(dt);
	animation.Update(dt);

	SpriteGO::Update(dt);
}

void Player::OnCollisionEnter(Collider* col)
{
	isJump = false;
	if (INPUT.GetKey(sf::Keyboard::A) || INPUT.GetKey(sf::Keyboard::D) ||
		INPUT.GetKey(sf::Keyboard::Left) || INPUT.GetKey(sf::Keyboard::Right))
	{
		animation.Play("Move");
	}
	else 
	{
		animation.Play("Idle");
	}
}

