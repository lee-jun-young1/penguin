#include "stdafx.h"
#include "ObstacleManager.h"
#include <SceneManager.h>

void ObstacleManager::Init()
{
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
}

void ObstacleManager::Update(float dt)
{
	time += dt;
	if (time > cycle)
	{
		time -= cycle;
		int random = Utils::RandomRange(0, 3);
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
	crevassePool.Return(crevasse);
}

IceHole& ObstacleManager::GetIceHole()
{
	return *iceHolePool.Get();
}

void ObstacleManager::ReturnIceHole(IceHole* iceHole)
{
	iceHolePool.Return(iceHole);
}

void ObstacleManager::ReturnAll()
{

}
