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

IceHole& StageManager::GetIceHole()
{
	cout << "iceHolePool get :: " << iceHolePool.GetUseList().size() + 1 << endl;
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

	cout << "iceHolePool return :: " << iceHolePool.GetUseList().size() << endl;
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
