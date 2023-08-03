#include "stdafx.h"
#include "DemoStageManager.h"
#include <SceneManager.h>
#include <sstream>
#include <GameDataManager.h>
#include <Penta.h>

DemoStageManager::DemoStageManager(const std::string& name)
	:StageManager(name)
{
}

void DemoStageManager::Reset()
{
	state = StageState::Playing;


	timeLimitAlert->SetClip(Resources.GetSoundBuffer("sound/sfx/10_NearTimeLimit.wav"));
	iceStation->Reset();
	iceStation->SetActive(false);
	background = (Background*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Background");
	bgm = (AudioSource*)background->GetComponent(ComponentType::Audio);
	bgm->SetClip(Resources.GetSoundBuffer("sound/bg/2_MainBgm.ogg"));
	bgm->SetLoop(false);
	bgm->Play();


	SCENE_MANAGER.GetCurrentScene()->FindGameObject("MapBG")->SetActive(true);
	SCENE_MANAGER.GetCurrentScene()->FindGameObject("Map")->SetActive(true);

	SetSpeedLevel(4);

	updateFunc = std::bind(&DemoStageManager::UpdateViewMap, this, std::placeholders::_1);
}

void DemoStageManager::UpdateViewMap(float dt)
{
	refreshTime += dt;
	if (refreshTime > 3.0f)
	{
		SCENE_MANAGER.GetCurrentScene()->FindGameObject("MapBG")->SetActive(false);
		SCENE_MANAGER.GetCurrentScene()->FindGameObject("Map")->SetActive(false);

		bgm = (AudioSource*)background->GetComponent(ComponentType::Audio);
		bgm->SetClip(Resources.GetSoundBuffer("sound/bg/2_MainBgm.ogg"));
		bgm->SetLoop(false);
		bgm->Play();

		Penta* player = (Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player");
		player->StartStage();

		SetSpeedLevel(4);
		updateFunc = std::bind(&StageManager::UpdatePlaying, this, std::placeholders::_1);
	}
}

void DemoStageManager::Init()
{
	hurdleManager.Init();
	iceStation = new IceStation();
	iceStation->SetManager(this);
	iceStation->SetDirection({ FRAMEWORK.GetWindowSize().x * 0.5f, 55.0f }, { FRAMEWORK.GetWindowSize().x * 0.5f, 115.0f });
	iceStation->Init();

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