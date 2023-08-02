#pragma once
#include "IceHole.h"
#include "Crevasse.h"
#include "Fish.h"
#include "FlagItem.h"
#include "ObjectPool.h"
#include <Framework.h>
class HurdleManager : GameObject
{
private:
	StageManager* stageManager;
	ObjectPool<Crevasse> crevassePool;
	ObjectPool<IceHole> iceHolePool;
	ObjectPool<Fish> fishPool;
	ObjectPool<FlagItem> flagPool;

	list<GameObject*> manageObjects;
	vector<GameObject*> removeManageObjects;

	//TODO ReadFile
	sf::Vector2f startXRange = { FRAMEWORK.GetWindowSize().x * 0.45f, FRAMEWORK.GetWindowSize().x * 0.55f };
	float startY = 55.0f;
	sf::Vector2f endXRange = { FRAMEWORK.GetWindowSize().x * 0.2f, FRAMEWORK.GetWindowSize().x * 0.8f };
	float endY = 165.0f;
public:
	void CreateObj();
	void RemoveObj();
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void UpdateComponent(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;
	Crevasse* GetCrevasse();
	void ReturnCrevasse(Crevasse* crevasse);
	Fish* GetFish();
	void ReturnFish(Fish* fish);
	IceHole* GetIceHole();
	void ReturnIceHole(IceHole* iceHole);
	FlagItem* GetFlag();
	void ReturnFlag(FlagItem* flag);
	void ReturnAll();
	void RemoveManageObject(GameObject* go);


	void SetStageManager(StageManager* manager){this->stageManager = manager;}
};

