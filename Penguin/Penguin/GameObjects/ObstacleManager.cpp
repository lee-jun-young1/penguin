#include "stdafx.h"
#include "ObstacleManager.h"
#include <SceneManager.h>
#include "InputManager.h"
#include <SpriteTextGO.h>
#include <sstream>
void ObstacleManager::Init()
{
	crevassePool.OnCreate = [this](Crevasse* crevasse) { crevasse->SetManager(this); };
	iceHolePool.OnCreate = [this](IceHole* iceHole) 
	{
		iceHole->SetManager(this);
		if (Utils::RandomRange(0, 2) == 1)
		{
			Seal* seal = new Seal();
			seal->Init();
			iceHole->SetSeal(seal);
			seal->SetManager(this);
		}
	};
	crevassePool.Init(20);
	iceHolePool.Init(20);
}

void ObstacleManager::Release()
{
	crevassePool.Release();
	iceHolePool.Release();
}

void ObstacleManager::Reset()
{
	SetSpeedLevel(4);
}

void ObstacleManager::Update(float dt)
{
	time += dt * GetSpeed() * 2.0f;
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
			int t = Utils::RandomRange(0, 3);
			crevasse->SetDirection({ Utils::RandomRange(startXRange.x, startXRange.y), startY }, { Utils::RandomRange(endXRange.x, endXRange.y), endY });
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(crevasse);
		}
			break;
		case 2:
		{
			IceHole* iceHole = &GetIceHole();
			iceHole->SetDirection({ Utils::RandomRange(startXRange.x, startXRange.y), startY }, { Utils::RandomRange(endXRange.x, endXRange.y), endY });
			SCENE_MANAGER.GetCurrentScene()->AddGameObject(iceHole );
			Seal* seal = iceHole->GetSeal();
			if (seal != nullptr)
			{
				SCENE_MANAGER.GetCurrentScene()->AddGameObject(seal);
				seal->Reset();
			}
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

void ObstacleManager::Draw(sf::RenderWindow& window)
{
}

void ObstacleManager::OnGUI(sf::RenderWindow& window)
{
}

Crevasse& ObstacleManager::GetCrevasse()
{
	return *crevassePool.Get();
}

void ObstacleManager::ReturnCrevasse(Crevasse* crevasse)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(crevasse);
	crevassePool.Return(crevasse);
}

IceHole& ObstacleManager::GetIceHole()
{
	return *iceHolePool.Get();
}

void ObstacleManager::ReturnIceHole(IceHole* iceHole)
{
	if (iceHole->GetSeal() != nullptr)
	{
		SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole->GetSeal());
	}
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole);
	iceHolePool.Return(iceHole);
}

void ObstacleManager::ReturnAll()
{

}

void ObstacleManager::IncreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(++speedLevel, 0, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}

void ObstacleManager::DecreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(--speedLevel, 1, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}

void ObstacleManager::SetSpeedLevel(int i)
{
	speedLevel = Utils::Clamp(i, 0, speedLevelMax);
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "SPEED " << speedLevel;
	speedText->SetText(ss.str());
}
