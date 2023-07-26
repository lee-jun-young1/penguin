#include "stdafx.h"
#include "PentaBot.h"
#include "BoxCollider.h"
#include "RigidBody2D.h"
#include <InputManager.h>
#include <SceneDemo.h>
#include <SceneManager.h>

void PentaBot::Reset()
{
	SpriteGO::Reset();
	state = PentaState::Move;

	SetOrigin(Origins::BC);
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ position.x, position.y, 30.0f, 3.0f });
	boxCol->SetOffset({ 30.0f * -0.5f, -3.0f });

	rigidBody->SetVelocity({ 0.0f, 0.0f });
	rigidBody->SetGravity(true);
	SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, 165.0f);
	spriteDirection = { 1.0f, 0.0f };
	direction.y = 1.0f;
	hitHighSound = Resources.GetSoundBuffer("sound/sfx/2_Collided_High.wav");
	hitLowSound = Resources.GetSoundBuffer("sound/sfx/3_Collided_Low.wav");
	fishSound = Resources.GetSoundBuffer("sound/sfx/4_Fish.wav");
	flagSound = Resources.GetSoundBuffer("sound/sfx/5_Flag.wav");
	jumpSound = Resources.GetSoundBuffer("sound/sfx/6_Jump.wav");
	crevasseSound = Resources.GetSoundBuffer("sound/sfx/7_InCrevasse.wav");
	pegicopterSound = Resources.GetSoundBuffer("sound/sfx/13_Pegicopter.wav");
	if (fishSound == nullptr)
	{
		cout << "Reset Error" << endl;
	}

	hasPegicopter = false;
	pegicopter->SetActive(false);
	afterClearTime = 0.0f;
#pragma region AnimationAction
	AnimationClip* hitClip = animator->GetState("Hit")->clip;

	hitClip->frames[0].action =
		[this]()
	{
		if (state == PentaState::Move)
		{
			rigidBody->AddForce({ 0.0f, -40.0f });
		}
		audio->SetClip(hitHighSound);
		audio->Play();
	};
	hitClip->frames[1].action =
		[this]()
	{
		rigidBody->AddForce({ 0.0f, -40.0f });
		audio->SetClip(hitLowSound);
		audio->Play();
	};
	hitClip->frames[2].action =
		[this]()
	{
		rigidBody->AddForce({ 0.0f, -40.0f });
		audio->SetClip(hitLowSound);
		audio->Play();
	};
	hitClip->frames[3].action =
		[this]()
	{
		animator->SetEvent("HitEnd");
		SetFlipX(false);
		updateFunc = std::bind(&PentaBot::UpdateMove, this, std::placeholders::_1);
		state = PentaState::Move;
	};

	AnimationClip* CrevasseIdle = animator->GetState("CrevasseIdle")->clip;
	CrevasseIdle->frames[0].action =
		[this]()
	{
		SetOrigin(GetSize().x * 0.5f, GetSize().y * 0.75f);
	};
	CrevasseIdle->frames[2].action =
		[this]()
	{
		animator->SetEvent("Move");
	};
	AnimationClip* CrevasseMove = animator->GetState("CrevasseMove")->clip;
	CrevasseMove->frames[0].action =
		[this]()
	{
		SetOrigin(Origins::BC);
	};
	for (int i = 1; i < CrevasseMove->frames.size() - 1; i += 2)
	{
		CrevasseMove->frames[i].action =
			[this]()
		{
			SetOrigin(GetSize().x * 0.5f, GetSize().y * 0.9f);
		};
		CrevasseMove->frames[i + 1].action =
			[this]()
		{
			SetOrigin(Origins::BC);
		};
	}
	CrevasseMove->frames[11].action =
		[this]()
	{
		animator->SetEvent("Idle");
	};

#pragma endregion

	updateFunc = std::bind(&PentaBot::UpdateMove, this, std::placeholders::_1);
}

void PentaBot::Update(float dt)
{
	SpriteGO::Update(dt);
	if (dt == 0.0f)
	{
		return;
	}

	if (fishSound == nullptr)
	{
		cout << "Update Error" << endl;
	}
	updateFunc(dt);
	pegicopter->SetPosition(GetPosition());
}


void PentaBot::UpdateHit(float deltaTime)
{
	rigidBody->SetVelocity({ direction.x * speed * 0.5f, rigidBody->GetVelocity().y });
}
void PentaBot::UpdateCrevasse(float deltaTime)
{
	actionCooldown -= deltaTime;
	SetPosition(position.x, crevasse->GetPosition().y);
	if (actionCooldown <= 0.0f && animator->GetClipName() == "CrevasseMove")
	{
		cout << "PENTABOT :: " << "Escape!!" << endl;
		actionCooldown = 0.5f;
		animator->SetEvent("Escape");
		state = PentaState::Move;
		SetPosition(position.x, 165.0f);
		SetOrigin(Origins::BC);
		updateFunc = std::bind(&PentaBot::UpdateMove, this, std::placeholders::_1);

		shadow->SetActive(true);

		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->SetSpeedLevel(1);
	}
}
void PentaBot::UpdateMove(float deltaTime)
{
	actionCooldown -= deltaTime;
	if (actionCooldown > 0.0f)
	{
		return;
	}

	rigidBody->SetVelocity({ 0.0f * speed + centrifugalForceDirection * speed * 0.7f, rigidBody->GetVelocity().y });

	int random = Utils::RandomRange(0, 9);
	
	switch (random)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	{
		cout << "PENTABOT :: " << "Increase Speed!!" << endl;
		actionCooldown = 0.1f;
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->IncreaseSpeedLevel();
	}
	break;
	case 4:
	case 5:
	{
		cout << "PENTABOT :: " << "Decrease Speed!!" << endl;
		actionCooldown = 0.1f;
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->DecreaseSpeedLevel();
	}
		break;
	case 6:
	{
		actionCooldown = 0.1f;
		if (state == PentaState::Move)
		{
			cout << "PENTABOT :: " << "Jump!!" << endl;
			//Jump
			rigidBody->AddForce({ 0.0f, -100.0f });
			animator->SetEvent("Jump");
			audio->SetClip(jumpSound);
			audio->Play();
			state = PentaState::Jump;
			updateFunc = std::bind(&PentaBot::UpdateJump, this, std::placeholders::_1);
		}
	}
		break;
	case 7:
		cout << "PENTABOT :: " << "Goto Left!!" << endl;
		actionCooldown = 1.0f;
		rigidBody->SetVelocity({ -1.0f * speed + centrifugalForceDirection * speed * 0.7f, rigidBody->GetVelocity().y });
		break;
	case 8:
		cout << "PENTABOT :: " << "Goto Right!!" << endl;
		actionCooldown = 1.0f;
		rigidBody->SetVelocity({ 1.0f * speed + centrifugalForceDirection * speed * 0.7f, rigidBody->GetVelocity().y });
		break;
	}
}

void PentaBot::UpdateClear(float deltaTime)
{
	afterClearTime += deltaTime;
	SetPosition(Utils::Lerp(clearPosition, clearTargetPosition, afterClearTime));
	if (afterClearTime > 1.0f)
	{
		animator->SetEvent("Clear");
	}
}

void PentaBot::UpdateJump(float deltaTime)
{
	if (hasPegicopter && rigidBody->GetVelocity().y < 0.0f)
	{
		cout << "PENTABOT :: " << "Use Item!!" << endl;
		state = PentaState::Pegicopter;
		pegicopterTime = pegicopterDuration;
		pegicopterAni->SetEvent("Use");
		direction.x = 0.0f;
		rigidBody->SetVelocity({ 0.0f, rigidBody->GetVelocity().y });
		hasPegicopter = false;
		updateFunc = std::bind(&PentaBot::UpdatePegicopter, this, std::placeholders::_1);
		return;
	}
}

void PentaBot::UpdatePegicopter(float deltaTime)
{
	if (rigidBody->GetVelocity().y >= 0.0f)
	{
		if (!audio->IsPlaying())
		{
			audio->SetClip(pegicopterSound);
			audio->Play();
		}
		pegicopterTime -= deltaTime;
		if (pegicopterTime < 0.0f)
		{
			rigidBody->SetGravity(true);
			pegicopterAni->SetEvent("Idle");
			pegicopter->SetActive(false);

			audio->Stop();
			state = PentaState::Jump;
			updateFunc = std::bind(&PentaBot::UpdateJump, this, std::placeholders::_1);
		}
		else
		{
			rigidBody->SetGravity(false);
			rigidBody->SetVelocity({ centrifugalForceDirection * speed, 0.0f });
		}
		return;
	}
}

void PentaBot::OnCollisionEnter(Collider* col)
{
	if (state == PentaState::Move || state == PentaState::Jump || state == PentaState::Hit)
	{
		if (col->GetGameObject().GetName() == "Seal" || (((col->GetGameObject().GetName() == "IceHole" || col->GetGameObject().GetName() == "CrevasseSide") && rigidBody->GetVelocity().y >= 0.0f)))
		{
			animator->SetEvent("Hit");
			state = PentaState::Hit;
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
			updateFunc = std::bind(&Penta::UpdateHit, this, std::placeholders::_1);
			Scene* scene = SCENE_MANAGER.GetCurrentScene();
			SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
			gameScene->GetStageManager()->SetSpeedLevel(1);
		}
		else if (col->GetGameObject().GetName() == "CrevasseCenter" && rigidBody->GetVelocity().y >= 0.0f)
		{
			animator->SetEvent("Crevasse");
			crevasse = col->GetGameObject().GetParent();
			state = PentaState::InHole;
			rigidBody->SetVelocity({ 0.0f, 0.0f });
			direction.x = 0.0f;
			updateFunc = std::bind(&Penta::UpdateCrevasse, this, std::placeholders::_1);
			actionCooldown = 3.0f;
			audio->SetClip(crevasseSound);
			audio->Play();

			shadow->SetActive(false);

			Scene* scene = SCENE_MANAGER.GetCurrentScene();
			SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
			gameScene->GetStageManager()->SetSpeedLevel(0);
		}
	}
	if (state == PentaState::Jump && col->GetGameObject().GetName() == "Ground")
	{
		state = PentaState::Move;
		animator->SetEvent("Move");
		updateFunc = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
	}


}
