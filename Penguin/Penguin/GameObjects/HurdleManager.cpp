#include "stdafx.h"
#include "HurdleManager.h"
#include <Utils.h>
#include <InputManager.h>
#include <SceneManager.h>

void HurdleManager::CreateObj()
{
	int random = Utils::RandomRange(0, 4);

	//cout << "gen " << name << endl;
#ifdef _DEBUG
	if (Input.GetKey(sf::Keyboard::F5))
	{
		random = 1;
	}
	if (Input.GetKey(sf::Keyboard::F6))
	{
		random = 2;
	}
	if (Input.GetKey(sf::Keyboard::F7))
	{
		random = 3;
	}
#endif
	switch (random)
	{
	case 0:
		break;
	case 1:
	{
		Crevasse* crevasse = GetCrevasse();
		float t = Utils::RandomValue();
		crevasse->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(crevasse);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(crevasse);
	}
	break;
	case 2:
	{
		IceHole* iceHole = GetIceHole();
		float t = Utils::RandomValue();
		iceHole->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(iceHole);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(iceHole);

		Seal* seal = iceHole->GetSeal();
		if (seal != nullptr)
		{
			seal->SetActive(true);
			manageObjects.push_back(seal);
			//SCENE_MANAGER.GetCurrentScene()->AddGameObject(seal);
			seal->Reset();
		}
	}
	break;
	case 3:
	{
		FlagItem* flag = GetFlag();
		float t = Utils::RandomValue();
		flag->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(flag);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(flag);
	}
	break;
	}
}


void HurdleManager::RemoveObj()
{
	for (auto removeObj : removeManageObjects)
	{
		manageObjects.remove(removeObj);
	}
	removeManageObjects.clear();
}

void HurdleManager::Init()
{
	crevassePool.OnCreate = [this](Crevasse* crevasse) { crevasse->SetManager(stageManager); };
	iceHolePool.OnCreate = [this](IceHole* iceHole)
	{
		iceHole->SetManager(stageManager);
		int random = Utils::RandomRange(0, 4);
		switch (random)
		{
		case 0:
		{
			Seal* seal = new Seal();
			seal->Init();
			iceHole->SetSeal(seal);
			seal->SetManager(stageManager);
		}
		break;
		}
	};
	fishPool.OnCreate = [this](Fish* fish) { fish->SetManager(stageManager); };
	flagPool.OnCreate = [this](FlagItem* flag) { flag->SetManager(stageManager); };
	crevassePool.Init(20);
	iceHolePool.Init(20);
	fishPool.Init(20);
	flagPool.Init(20);
}

void HurdleManager::Release()
{
	crevassePool.Release();
	iceHolePool.Release();
	fishPool.Release();
	flagPool.Release();
}

void HurdleManager::Update(float dt)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->Update(dt);
		}
	}
}

void HurdleManager::UpdateComponent(float dt)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->UpdateComponent(dt);
		}
	}
}
void HurdleManager::Draw(sf::RenderWindow& window)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->Draw(window);
		}
	}
}

void HurdleManager::OnGUI(sf::RenderWindow& window)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->OnGUI(window);
		}
	}
}

Crevasse* HurdleManager::GetCrevasse()
{
	//cout << "crevassePool get :: " << crevassePool.GetUseList().size() + 1 << endl;
	return crevassePool.Get();
}

void HurdleManager::ReturnCrevasse(Crevasse* crevasse)
{
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(crevasse);
	RemoveManageObject(crevasse);
	crevassePool.Return(crevasse);
	//cout << "crevassePool return :: " << crevassePool.GetUseList().size() << endl;
}

Fish* HurdleManager::GetFish()
{
	return fishPool.Get();
}

void HurdleManager::ReturnFish(Fish* fish)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(fish);
	fishPool.Return(fish);
	//cout << "fishReturn" << fishPool.GetUseList().size() << endl;
}

IceHole* HurdleManager::GetIceHole()
{
	return iceHolePool.Get();
}

void HurdleManager::ReturnIceHole(IceHole* iceHole)
{
	if (iceHole->GetSeal() != nullptr)
	{
		RemoveManageObject(iceHole->GetSeal());
		//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole->GetSeal());
	}
	RemoveManageObject(iceHole);
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole);
	iceHolePool.Return(iceHole);
}

FlagItem* HurdleManager::GetFlag()
{
	//cout << "flag Get" << flagPool.GetUseList().size() << endl;
	return flagPool.Get();
}

void HurdleManager::ReturnFlag(FlagItem* flag)
{
	//cout << "flag Return" << flagPool.GetUseList().size() << endl;
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(flag);
	RemoveManageObject(flag);
	flagPool.Return(flag);
}

void HurdleManager::ReturnAll()
{

	for (auto& crevasse : crevassePool.GetUseList())
	{
		crevasse->Reset();
		RemoveManageObject(crevasse);
	}
	for (auto& iceHole : iceHolePool.GetUseList())
	{
		if (iceHole->GetSeal() != nullptr)
		{
			iceHole->GetSeal()->Reset();
			RemoveManageObject(iceHole->GetSeal());
		}
		iceHole->Reset();
		RemoveManageObject(iceHole);
	}
	for (auto& fish : fishPool.GetUseList())
	{
		fish->Reset();
		SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(fish);
	}
	for (auto& flag : flagPool.GetUseList())
	{
		flag->Reset();
		RemoveManageObject(flag);
	}

	crevassePool.Clear();
	iceHolePool.Clear();
	fishPool.Clear();
	flagPool.Clear();
}

void HurdleManager::RemoveManageObject(GameObject* go)
{
	removeManageObjects.push_back(go);
}
