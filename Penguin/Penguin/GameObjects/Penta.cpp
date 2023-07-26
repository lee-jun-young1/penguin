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
	audio = new AudioSource(*this);
	AddComponent(audio);

	SetOrigin(Origins::BC);
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ position.x, position.y, 30.0f, 3.0f });
	boxCol->SetOffset({ 30.0f * -0.5f, -3.0f});
	rigidBody->SetVelocity({ 0.0f, 0.0f });
	rigidBody->SetGravity(false);
}

void Penta::Reset()
{
	SpriteGO::Reset();
	state = PentaState::Move;

	shadow->SetActive(true);
	centrifugalForceDirection = 0.0f;

	rigidBody->SetVelocity({ 0.0f, 0.0f });
	rigidBody->SetGravity(true);

	SetOrigin(Origins::BC);
	BoxCollider* boxCol = (BoxCollider*)GetComponent(ComponentType::Collider);
	boxCol->SetRect({ position.x, position.y, 30.0f, 3.0f });
	boxCol->SetOffset({ 30.0f * -0.5f, -3.0f });

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
		updateFunc = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
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

	updateFunc = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);
}

void Penta::Update(float dt)
{
	SpriteGO::Update(dt);
	if (dt == 0.0f)
	{
		return;
	}
	updateFunc(dt);
	pegicopter->SetPosition(GetPosition());
}

void Penta::UpdateHit(float deltaTime)
{
	rigidBody->SetVelocity({ direction.x * speed * 0.5f, rigidBody->GetVelocity().y });
}
void Penta::UpdateCrevasse(float deltaTime)
{
	SetPosition(position.x, crevasse->GetPosition().y);
	if (Input.GetKeyDown(sf::Keyboard::Space) && animator->GetClipName() == "CrevasseMove")
	{
		animator->SetEvent("Escape");
		state = PentaState::Move;
		SetPosition(position.x, 165.0f);
		SetOrigin(Origins::BC);
		updateFunc = std::bind(&Penta::UpdateMove, this, std::placeholders::_1);

		shadow->SetActive(true);

		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->SetSpeedLevel(1);
	}
}
void Penta::UpdateMove(float deltaTime)
{
	if (Input.GetKeyDown(sf::Keyboard::Up))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->IncreaseSpeedLevel();
	}
	if (Input.GetKeyDown(sf::Keyboard::Down))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->DecreaseSpeedLevel();
	}
	if (Input.GetKeyDown(sf::Keyboard::Space))
	{
		if (state == PentaState::Move) 
		{
			//Jump
			rigidBody->AddForce({ 0.0f, -100.0f });
			animator->SetEvent("Jump");
			audio->SetClip(jumpSound);
			audio->Play(); 
			state = PentaState::Jump;
			updateFunc = std::bind(&Penta::UpdateJump, this, std::placeholders::_1);
		}
	}
	Vector2f axis = { Input.GetAxisRaw(Axis::Horizontal), 0.0f };

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
	rigidBody->SetVelocity({ axis.x * speed + centrifugalForceDirection * speed * 0.7f, rigidBody->GetVelocity().y });
}

void Penta::UpdateClear(float deltaTime)
{
	afterClearTime += deltaTime;
	SetPosition(Utils::Lerp(clearPosition, clearTargetPosition, afterClearTime));
	if(afterClearTime > 1.0f)
	{
		animator->SetEvent("Clear");
	}
}

void Penta::UpdateJump(float deltaTime)
{
	if (Input.GetKeyDown(sf::Keyboard::Up))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->IncreaseSpeedLevel();
	}
	if (Input.GetKeyDown(sf::Keyboard::Down))
	{
		Scene* scene = SCENE_MANAGER.GetCurrentScene();
		SceneGame* gameScene = dynamic_cast<SceneGame*>(scene);
		gameScene->GetStageManager()->DecreaseSpeedLevel();
	}
	if (Input.GetKeyDown(sf::Keyboard::Space) && hasPegicopter && rigidBody->GetVelocity().y < 0.0f)
	{
		state = PentaState::Pegicopter;
		pegicopterTime = pegicopterDuration;
		pegicopterAni->SetEvent("Use");
		direction.x = 0.0f;
		rigidBody->SetVelocity({ 0.0f, rigidBody->GetVelocity().y });
		hasPegicopter = false;
		updateFunc = std::bind(&Penta::UpdatePegicopter, this, std::placeholders::_1);
		return;
	}
}

void Penta::UpdatePegicopter(float deltaTime)
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
			updateFunc = std::bind(&Penta::UpdateJump, this, std::placeholders::_1);
		}
		else
		{
			rigidBody->SetGravity(false);
			rigidBody->SetVelocity({ centrifugalForceDirection * speed, 0.0f });
		}
		return;
	}
}

void Penta::Draw(sf::RenderWindow& window)
{
	SpriteGO::Draw(window);
}

void Penta::OnCollisionEnter(Collider* col)
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

			shadow->SetActive(false);

			audio->SetClip(crevasseSound);
			audio->Play();

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

void Penta::SetOrigin(Origins origin)
{
	SpriteGO::SetOrigin(origin);
	sf::Vector2f originPos(sprite.getLocalBounds().width, sprite.getLocalBounds().height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;
	pegicopter->SetOrigin(pegicopter->sprite.getGlobalBounds().width * pegicopterOrigin.x, sprite.getGlobalBounds().height * pegicopterOrigin.y - (GetSize().y - originPos.y));
 	//pegicopter->SetOrigin(pegicopter->sprite.getGlobalBounds().width * 0.5f - (GetSize().x - originPos.x), sprite.getGlobalBounds().height * 1.05f - (GetSize().y - originPos.y));
}

void Penta::SetOrigin(float originX, float originY)
{
	SpriteGO::SetOrigin(originX, originY); 
	pegicopter->SetOrigin(pegicopter->sprite.getGlobalBounds().width * pegicopterOrigin.x, sprite.getGlobalBounds().height * pegicopterOrigin.y - (GetSize().y - originY));
	//pegicopter->SetOrigin(pegicopter->sprite.getGlobalBounds().width * 0.5f - (GetSize().x - originX), sprite.getGlobalBounds().height * 1.05f - (GetSize().y - originY));
}

void Penta::GetPegicopterItem()
{
	hasPegicopter = true;
	pegicopter->SetActive(true);
	pegicopterAni->SetEvent("Idle");
}

void Penta::SetPegicopter(SpriteGO* pegicopter, Animator* pegicopterAni)
{
	this->pegicopter = pegicopter;
	this->pegicopterAni = pegicopterAni;
	pegicopter->SetActive(false);
}

void Penta::Clear()
{
	state = PentaState::Clear;
	updateFunc = std::bind(&Penta::UpdateClear, this, std::placeholders::_1);
	pegicopter->SetActive(false);
	animator->SetEvent("Move");
	afterClearTime = 0.0f;
	clearPosition = position;
	rigidBody->SetVelocity({ 0.0f, 0.0f });
	rigidBody->SetGravity(false);
	direction.x = 0.0f;
}

void Penta::PlayFlagSound() 
{
	audio->SetClip(flagSound);
	audio->Play();
}


void Penta::PlayFishSound()
{
	audio->SetClip(fishSound);
	audio->Play();
}