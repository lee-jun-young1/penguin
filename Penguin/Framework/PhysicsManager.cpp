#include "stdafx.h"
#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	for (int i = 0; i < (int)PhysicsLayer::Count; i++)
	{
		layerCollide.push_back(0);
		colliders.push_back(std::list<Collider*>());
	}
	layerCollide[0] = 1 << 0;
}

std::list<Collider*> PhysicsManager::GetColliders(const int& physicsLayer)
{
	std::list<Collider*> result;
	for (int i = 0; i < (int)PhysicsLayer::Count; i++)
	{
		if (layerCollide[physicsLayer] & 1 << i && colliders[i].size() > 0)
		{
			auto it = colliders[i].begin();
			while (it != colliders[i].end())
			{
				result.push_back(*it);
				it++;
			}
		}
	}
	return result;
}

void PhysicsManager::AddColliders(Collider* col, const int& physicsLayer)
{
	colliders[physicsLayer].push_back(col);
}

void PhysicsManager::RemoveColliders(Collider* col, const int& physicsLayer)
{
	colliders[physicsLayer].remove(col);
}
