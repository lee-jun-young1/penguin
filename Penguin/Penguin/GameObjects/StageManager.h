#pragma once
#include "GameObject.h"
#include "ObjectPool.h"
#include <Crevasse.h>
#include <IceHole.h>
#include <Utils.h>
#include <Framework.h>
#include "Fish.h"
#include "FlagItem.h"
#include "SpriteTextGO.h"
#include <AudioSource.h>
#include <IceStation.h>
#include <Background.h>

enum class StageState
{
	Playing,
	Clear,
	Timeout,
};

class StageManager : public GameObject
{
protected:
	StageState state;

	ObjectPool<Crevasse> crevassePool;
	ObjectPool<IceHole> iceHolePool;
	ObjectPool<Fish> fishPool;
	ObjectPool<FlagItem> flagPool;

	IceStation* iceStation;

	list<GameObject*> manageObjects;
	vector<GameObject*> removeManageObjects;

	int stage = 1;
	string stageName;

	vector<int> scoreTable = {300, 500, 1000};
	float stageTime = 10.0f;
	float stageRest = 10.0f;

	float genCycle = 0.5f;
	float genTime = 0.0f;

	float refreshCycle = 0.2f; //0.2f
	float refreshTime = 0.0f;

	float effectTime = 0.0f;

	const float defaultSpeed = 0.3f;
	float speed = 0.1f;

	const int speedLevelMax = 12;
	int speedLevel = 1;

	//TODO ReadFile
	sf::Vector2f startXRange = { FRAMEWORK.GetWindowSize().x * 0.45f, FRAMEWORK.GetWindowSize().x * 0.55f };
	float startY = 55.0f;
	sf::Vector2f endXRange = { FRAMEWORK.GetWindowSize().x * 0.2f, FRAMEWORK.GetWindowSize().x * 0.8f };
	float endY = 165.0f;

	const float flagItemPercentage = 0.1f;

	int score = 0;

	SpriteTextGO* timeText;
	SpriteTextGO* restText;
	AudioSource* timeLimitAlert;

	Background* background;
	AudioSource* bgm;

	std::function<void(float)> updateFunc;
public:
	StageManager(const std::string& name = "StageManager");

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override; //ÃÊ±âÈ­


	virtual void UpdateComponent(float dt);

	virtual void Update(float dt) override;
	void UpdatePlaying(float dt);
	virtual void CreateObj();
	void UpdateTimeOut(float dt);
	void OnExitScene();
	void UpdateClear(float dt);
	void RemoveObj();
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;

	Crevasse* GetCrevasse();
	void ReturnCrevasse(Crevasse* crevasse);
	IceHole* GetIceHole();
	void ReturnIceHole(IceHole* iceHole);
	FlagItem* GetFlag();
	void ReturnFlag(FlagItem* flag);
	Fish* GetFish();
	void ReturnFish(Fish* fish);

	void ReturnAll();

	const float GetSpeed() const { return speedLevel == 0 ? 0.0f : defaultSpeed + (speed * speedLevel); };
	const float GetRest() const { return stageRest; }
	void IncreaseSpeedLevel();
	void RemoveManageObject(GameObject*);
	void DecreaseSpeedLevel();
	void SetSpeedLevel(const int& level);


	const float& GetFlagItemPercentage() { return flagItemPercentage; };

	void IncreaseScore(const int& score);
	void IncreaseScore(const ScoreItemType& type);

	void ResetScore();

	void SetTimeText(SpriteTextGO* timeText) {this->timeText = timeText;}
	void SetRestText(SpriteTextGO* restText) { this->restText = restText; }

	void IncreaseStage();
	void SetStage(const int& stage);
	const std::string& GetStageName() const;
	void StageClear();
};

