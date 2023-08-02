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
#include <HurdleManager.h>

enum class StageState
{
	Playing,
	Clear,
	Timeout,
};

class StageManager : public GameObject
{
protected:
	HurdleManager hurdleManager;

	StageState state;

	IceStation* iceStation;

	int stage = 1;
	string stageName;

	vector<int> scoreTable = {300, 500, 1000};
	float stageTime = 10.0f;
	float stageRest = 10.0f;

	float refreshCycle = 0.2f; //0.2f
	float refreshTime = 0.0f;

	float effectTime = 0.0f;

	const float defaultSpeed = 0.8f;
	float speed = 0.1f;

	const int speedLevelMax = 12;
	int speedLevel = 1;

	float genCycle = 0.5f;
	float genTime = 0.0f;

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
	void UpdateViewMap(float dt);
	void UpdatePlaying(float dt);
	void UpdateTimeOut(float dt);
	void OnExitScene();
	void UpdateClear(float dt);
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;


	const float GetSpeed() const { return speedLevel == 0 ? 0.0f : defaultSpeed + (speed * speedLevel); };
	const float GetRest() const { return stageRest; }
	void IncreaseSpeedLevel();
	void DecreaseSpeedLevel();
	void SetSpeedLevel(const int& level);


	const float& GetFlagItemPercentage() { return flagItemPercentage; };

	virtual void IncreaseScore(const int& score);
	virtual void IncreaseScore(const ScoreItemType& type);

	virtual void ResetScore();

	void SetTimeText(SpriteTextGO* timeText) {this->timeText = timeText;}
	void SetRestText(SpriteTextGO* restText) { this->restText = restText; }

	void IncreaseStage();
	void SetStage(const int& stage);
	const std::string& GetStageName() const;
	void StageClear();

	HurdleManager& GetHurdleManager() { return hurdleManager; };
};

