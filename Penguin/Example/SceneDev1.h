#pragma once
#include "Scene.h"
#include "ObjectPool.h"
#include <SpriteEffect.h>

class Player;
class VertexArrayGO;
class Zombie;
class Item;

class SceneDev1 : public Scene
{
protected:
public:
	SceneDev1();
	virtual ~SceneDev1() override;

	virtual void Enter() override;
	void Reset();
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);
};