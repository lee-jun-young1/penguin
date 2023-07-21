#include "stdafx.h"
#include "StageManager.h"
#include <SceneManager.h>
#include "InputManager.h"
#include <SpriteTextGO.h>
#include <sstream>
void StageManager::Init()
{
	crevassePool.OnCreate = [this](Crevasse* crevasse) { crevasse->SetManager(this); };
	iceHolePool.OnCreate = [this](IceHole* iceHole) 
	{
		iceHole->SetManager(this);
		int random = Utils::RandomRange(0, 4);
		switch(random)
		{
		case 0:
			{
				Seal* seal = new Seal();
				seal->Init();
				iceHole->SetSeal(seal);
				seal->SetManager(this);
			}
			break;
		}
	};
	fishPool.OnCreate = [this](Fish* fish) { fish->SetManager(this); };
	flagPool.OnCreate = [this](FlagItem* flag) { flag->SetManager(this); };
	crevassePool.Init(20);
	iceHolePool.Init(20);
	fishPool.Init(20);
}

void StageManager::Release()
{
	crevassePool.Release();
	iceHolePool.Release();
}

void StageManager::Reset()
{
	SetSpeedLevel(4);
}

void StageManager::Update(float dt)
{
	time += dt * GetSpeed();
	if (time > cycle)
	{
		time -= cycle;
		int random = Utils::RandomRange(0, 4);

		switch (random)
		{
		case 0:
			break;
		case 1:
		{
			Crevasse* crevasse = &GetCrevasse();
			float t = Utils::RandomValue();
			crevasse->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(crevasse);
		}
			break;
		case 2:
		{
			IceHole* iceHole = &GetIceHole();
			float t = Utils::RandomValue();
			iceHole->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(iceHole );
			Seal* seal = iceHole->GetSeal();

			if (seal != nullptr)
			{
				SCENE_MANAGER.GetCurrentScene()->AddGameObject(seal);
				seal->Reset();
			}
		}
			break;
		case 3:
		{
			FlagItem* flag = &GetFlag();
			float t = Utils::RandomValue();
			flag->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(flag);
		}
			break;
		}
	}

	for (auto crevasse : crevassePool.GetUseList())
	{
		if (!crevasse->IsActive())
		{
			crevassePool.Return(crevasse);
		}
	}
	for (auto iceHole : iceHolePool.GetUseList())
	{
		if (!iceHole->IsActive())
		{
			iceHolePool.Return(iceHole);
		}
	}

}

void StageManager::Draw(sf::RenderWindow& window)
{
}

void StageManager::OnGUI(sf::RenderWindow& window)
{
}

Crevasse& StageManager::GetCrevasse()
{
	cout << "crevassePool get :: " << crevassePool.GetUseList().size() + 1 << endl;
	return *crevassePool.Get();
}

void StageManager::ReturnCrevasse(Crevasse* crevasse)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(crevasse);
	crevassePool.Return(crevasse);
	cout << "crevassePool return :: " << crevassePool.GetUseList().size() << endl;
}

Fish& StageManager::GetFish()
{
 	return *fishPool.Get();
}

void StageManager::ReturnFish(Fish* fish)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(fish);
	fishPool.Return(fish);
}

IceHole& StageManager::GetIceHole()
{
	return *iceHolePool.Get();
}

void StageManager::ReturnIceHole(IceHole* iceHole)
{
	if (iceHole->GetSeal() != nullptr)
	{
		SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole->GetSeal());
	}
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole);
	iceHolePool.Return(iceHole);
}

FlagItem& StageManager::GetFlag()
{
	return *flagPool.Get();
}

void StageManager::ReturnFlag(FlagItem* flag)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(flag);
	flagPool.Return(flag);
}

void StageManager::ReturnAll()
{

}

void StageManager::IncreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(++speedLevel, 0, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}

void StageManager::DecreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(--speedLevel, 1, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}

void StageManager::SetSpeedLevel(int i)
{
	speedLevel = Utils::Clamp(i, 0, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}

void StageManager::IncreaseScore(int score)
{
	this->score += score;
	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "SCORE " << this->score;
	scoreText->SetText(ss.str());
}
