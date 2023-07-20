#include "stdafx.h"
#include "Penta.h"
#include "InputManager.h"
#include "RigidBody2D.h"
#include <Framework.h>
#include <BoxCollider.h>
#include <Utils.h>
#include "AnimationClip.h"
#include <SceneGame.h>
#include <SceneManager.h>

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
	state = State::Move;
	animator->Play();
	SetOrigin(origin);
	SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, 165.0f);
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
		if (state == State::Move)
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
		state = State::Move;
	};

	AnimationClip& CrevasseIdle = animator->GetState("CrevasseIdle")->clip;
	CrevasseIdle.frames[0].action =
		[this]()
	{
		SetOrigin(GetSize().x * 0.5f, GetSize().y * 0.75f);
	};
	CrevasseIdle.frames[4].action =
		[this]()
	{
		animator->SetEvent("Move");
	};
	AnimationClip& CrevasseMove = animator->GetState("CrevasseMove")->clip;
	CrevasseMove.frames[0].action =
		[this]()
	{
		SetOrigin(Origins::BC);
	};
	for (int i = 1; i < CrevasseMove.frames.size() - 1; i += 2)
	{
		CrevasseMove.frames[i].action =
			[this]()
		{
			SetOrigin(GetSize().x * 0.5f, GetSize().y * 0.9f);
		};
		CrevasseMove.frames[i + 1].action =
			[this]()
		{
			SetOrigin(Origins::BC);
		};
	}
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
	SetPosition(position.x, crevasse->GetPosition().y);
	if (INPUT.GetKeyDown(sf::Keyboard::Space) && animator->GetClipName() == "CrevasseMove")
	{
		animator->SetEvent("Escape");
		state = State::Move;
		SetPosition(position.x, 165.0f);
		SetOrigin(Origins::BC);
		stateUpdate = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);

		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->SetSpeedLevel(1);
	}
}
void Penta::UpdateMove(float deltaTime)
{
	if (INPUT.GetKeyDown(sf::Keyboard::Up))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->IncreaseSpeedLevel();
	}
	if (INPUT.GetKeyDown(sf::Keyboard::Down))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->DecreaseSpeedLevel();
	}
	if (state == State::Move && INPUT.GetKeyDown(sf::Keyboard::Space))
	{
		//Jump
		rigidBody->AddForce({ 0.0f, -100.0f });
		animator->SetEvent("Jump");
		audio->SetClip(jumpSound);
		audio->Play(); 
		state = State::Jump;
	}
	Vector2f axis = { INPUT.GetAxisRaw(Axis::Horizontal), 0.0f };

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
	if (state == State::Move || state == State::Jump || state == State::Hit)
	{
		if (col->GetGameObject().GetName() == "Seal" || (col->GetGameObject().GetName() == "IceHole" || col->GetGameObject().GetName() == "CrevasseSide" && rigidBody->GetVelocity().y >= 0.0f))
		{
			animator->SetEvent("Hit");
			state = State::Hit;
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
			Scene* scene = SCENE_MANAGER.GetCurrentScene();
			SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
			gameScene->GetStageManager()->SetSpeedLevel(1);
		}
		else if (col->GetGameObject().GetName() == "CrevasseCenter" && rigidBody->GetVelocity().y >= 0.0f)
		{
			animator->SetEvent("Crevasse");
			crevasse = col->GetGameObject().GetParent();
			state = State::InHole;
			rigidBody->SetVelocity({ 0.0f, 0.0f });
			direction.x = 0.0f;
			stateUpdate = std::bind(&Penta::UpdateCrevasse, this, std::placeholders::_1);

			Scene* scene = SCENE_MANAGER.GetCurrentScene();
			SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
			gameScene->GetStageManager()->SetSpeedLevel(0); 
		}
	}
	if (state == State::Jump && col->GetGameObject().GetName() == "Ground")
	{
		state = State::Move;
		animator->SetEvent("Move");
	}

	
}