#include "stdafx.h"
#include "DemoStageManager.h"
#include <SceneManager.h>
#include <sstream>
#include <GameDataManager.h>

DemoStageManager::DemoStageManager(const std::string& name)
	:StageManager(name)
{
}

void DemoStageManager::Init()
{
	crevassePool.OnCreate = [this](Crevasse* crevasse) { crevasse->SetManager(this); };
	iceHolePool.OnCreate = [this](IceHole* iceHole)
	{
		iceHole->SetManager(this);
		int random = Utils::RandomRange(0, 4);
		switch (random)
		{
		case 0:
		{
			Seal* seal = new Seal();
			seal->Init();
			iceHole->SetSeal(seal);
			seal->SetManager(this);
		}
		break;
		}
	};
	fishPool.OnCreate = [this](Fish* fish) { fish->SetManager(this); fish->SetName("DemoFish"); };
	flagPool.OnCreate = [this](FlagItem* flag) { flag->SetManager(this); flag->SetName("DemoFlag"); };
	crevassePool.Init(20);
	iceHolePool.Init(20);
	fishPool.Init(20);
	flagPool.Init(20);

	iceStation = new IceStation();
	iceStation->SetManager(this);
	iceStation->SetDirection({ FRAMEWORK.GetWindowSize().x * 0.5f, startY }, { FRAMEWORK.GetWindowSize().x * 0.5f, endY - 50.0f });
	iceStation->Init();
	manageObjects.push_back(iceStation);

	timeLimitAlert = new AudioSource(*this);
	timeLimitAlert->SetLoop(false);
	AddComponent(timeLimitAlert);
}

void DemoStageManager::IncreaseScore(const int& score)
{
	this->score += score;

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "AI-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}

void DemoStageManager::IncreaseScore(const ScoreItemType& type)
{
	this->score += scoreTable[(int)type];

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "AI-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}

void DemoStageManager::ResetScore()
{
	this->score = 0;

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "AI-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}