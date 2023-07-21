#pragma once
#include "GameObject.h"
#include "ObjectPool.h"
#include <Crevasse.h>
#include <IceHole.h>
#include <Utils.h>
#include <Framework.h>
#include "Fish.h"
#include "FlagItem.h"
class StageManager : public GameObject
{
private:
	ObjectPool<Crevasse> crevassePool;
	ObjectPool<IceHole> iceHolePool;
	ObjectPool<Fish> fishPool;
	ObjectPool<FlagItem> flagPool;

	float cycle = 0.5f;
	float time = 0.0f;

	const float defaultSpeed = 0.3f;
	float speed = 0.05f;

	const int speedLevelMax = 12;
	int speedLevel = 1;

	//TODO ReadFile
	sf::Vector2f startXRange = { FRAMEWORK.GetWindowSize().x * 0.4f, FRAMEWORK.GetWindowSize().x * 0.6f };
	float startY = 60.0f;
	sf::Vector2f endXRange = { FRAMEWORK.GetWindowSize().x * 0.2f, FRAMEWORK.GetWindowSize().x * 0.8f };
	float endY = 165.0f;

	int score = 0;
public:

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override; //√ ±‚»≠

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;

	Crevasse& GetCrevasse();
	void ReturnCrevasse(Crevasse* crevasse);
	IceHole& GetIceHole();
	void ReturnIceHole(IceHole* iceHole);
	FlagItem& GetFlag();
	void ReturnFlag(FlagItem* flag);

	Fish& GetFish();
	void ReturnFish(Fish* fish);

	void ReturnAll();

	float GetSpeed() { return speedLevel == 0 ? 0.0f : defaultSpeed + (speed * speedLevel); };
	void IncreaseSpeedLevel();
	void DecreaseSpeedLevel();
	void SetSpeedLevel(int i);

	void IncreaseScore(int score);
};

