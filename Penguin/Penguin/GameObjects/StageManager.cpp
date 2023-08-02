#include "stdafx.h"
#include "StageManager.h"
#include <SceneManager.h>
#include "InputManager.h"
#include <SpriteTextGO.h>
#include <sstream>
#include <RectangleShapeGO.h>
#include <DataTableManager.h>
#include <StageDataTable.h>
#include <Penta.h>
#include <GameDataManager.h>
StageManager::StageManager(const std::string& name)
	:GameObject(name)
{
	hurdleManager.SetStageManager(this);
}

void StageManager::Init()
{
	hurdleManager.Init();
	iceStation = new IceStation();
	iceStation->SetManager(this);
	iceStation->SetDirection({ FRAMEWORK.GetWindowSize().x * 0.5f, 55.0f }, { FRAMEWORK.GetWindowSize().x * 0.5f, 115.0f});
	iceStation->Init();

	timeLimitAlert = new AudioSource(*this);
	timeLimitAlert->SetLoop(false);
	AddComponent(timeLimitAlert);
}

void StageManager::Release()
{
	hurdleManager.Release();
}

void StageManager::Reset()
{
	state = StageState::Playing;


	timeLimitAlert->SetClip(Resources.GetSoundBuffer("sound/sfx/10_NearTimeLimit.wav"));

	iceStation->Reset();
	iceStation->SetActive(false);

	background = (Background*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Background");
	bgm = (AudioSource*)background->GetComponent(ComponentType::Audio);
	bgm->Stop();

	SCENE_MANAGER.GetCurrentScene()->FindGameObject("MapBG")->SetActive(true);
	SCENE_MANAGER.GetCurrentScene()->FindGameObject("Map")->SetActive(true);


	SetSpeedLevel(0);

	refreshTime = 0.0f;
	updateFunc = std::bind(&StageManager::UpdateViewMap, this, std::placeholders::_1);
}

void StageManager::UpdateComponent(float dt)
{
	hurdleManager.UpdateComponent(dt);
}

void StageManager::Update(float dt)
{
	//cout << "StageManager Update" << endl;
	updateFunc(dt);
}
void StageManager::UpdateViewMap(float dt)
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

void StageManager::UpdatePlaying(float dt)
{
	stageTime -= dt;
	stageRest -= dt * GetSpeed() * 10.0f;
#ifdef _DEBUG
	if (Input.GetKeyDown(sf::Keyboard::Numpad1))
	{
		stageTime -= 10.0f;
	}
	if (Input.GetKeyDown(sf::Keyboard::Numpad7))
	{
		stageTime += 10.0f;
	}
	if (Input.GetKeyDown(sf::Keyboard::Numpad3))
	{
		stageRest -= 100.0f;
	}
	if (Input.GetKeyDown(sf::Keyboard::Numpad9))
	{
		stageRest += 100.0f;
	}
#endif
	stringstream ss;
	ss << "TIME-" << Utils::ToString((int)stageTime, "0000");
	timeText->SetText(ss.str());


	ss.str("");

	ss << "wxyz_" << Utils::ToString((int)stageRest, "0000") << "km";
	restText->SetText(ss.str());

	if (stageRest <= 0.0f)
	{
		StageClear();
	}

	if (stageTime < 0.0f)
	{
		state = StageState::Timeout;
		updateFunc = std::bind(&StageManager::UpdateTimeOut, this, std::placeholders::_1);
		bgm->SetClip(Resources.GetSoundBuffer("sound/bg/4_TimeOver.ogg"));
		bgm->SetLoop(false);
		bgm->Play();

		FRAMEWORK.SetTimeScale(0.0f);
		SpriteTextGO* sysMsgText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsg");
		sysMsgText->SetText("TIME OUT");
		sysMsgText->SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.3f);
		sysMsgText->SetOrigin(Origins::MC);
		sysMsgText->SetActive(true);

		RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsgRect");
		sysMsgRect->SetSize(sysMsgText->GetSize());
		sysMsgRect->SetPosition(sysMsgText->GetPosition());
		sysMsgRect->SetOrigin(Origins::MC);
		sysMsgRect->SetActive(true);
		return;
	}
	else if (stageTime < 10.0f && !timeLimitAlert->IsPlaying())
	{
		timeLimitAlert->Play();
	}

	if (speedLevel != 0)
	{
		refreshTime += dt;
	}
	if (refreshTime * GetSpeed() < refreshCycle)
	{
		return;
	}

	genTime += refreshTime * GetSpeed();
	if (genTime > genCycle && stageRest > 50.0f)
	{
		genTime -= genCycle;
		hurdleManager.CreateObj();
	}
	else if (stageRest <= 50.0f)
	{
		iceStation->SetActive(true);
	}

	hurdleManager.Update(refreshTime);

	//for (auto crevasse : crevassePool.GetUseList())
	//{
	//	if (!crevasse->IsActive())
	//	{
	//		crevassePool.Return(crevasse);
	//	}
	//}
	//for (auto iceHole : iceHolePool.GetUseList())
	//{
	//	if (!iceHole->IsActive())
	//	{
	//		iceHolePool.Return(iceHole);
	//	}
	//}
	//for (auto fish : fishPool.GetUseList())
	//{
	//	if (!fish->IsActive())
	//	{
	//		fishPool.Return(fish);
	//	}
	//}
	//for (auto flag : flagPool.GetUseList())
	//{
	//	if (!flag->IsActive())
	//	{
	//		flagPool.Return(flag);
	//	}
	//}
	hurdleManager.RemoveObj();

	refreshTime = 0;
}

void StageManager::UpdateTimeOut(float dt)
{
	if (!bgm->IsPlaying())
	{
		FRAMEWORK.SetTimeScale(1.0f); 
		OnExitScene();
		SCENE_MANAGER.ChangeScene(SceneId::Title);
	}
}

void StageManager::OnExitScene()
{
	SpriteTextGO* sysMsgText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsg");
	RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsgRect");
	sysMsgText->SetActive(false);
	sysMsgRect->SetActive(false);
	GetHurdleManager().ReturnAll();
	hurdleManager.RemoveObj();
}

void StageManager::UpdateClear(float dt)
{
	iceStation->Update(dt);
	effectTime -= dt;
	if (effectTime < 0.0f)
	{
		effectTime += 0.1f;
		if (stageTime > 0.0f)
		{
			bgm->SetClip(Resources.GetSoundBuffer("sound/sfx/1_TimeScore.wav"));
			bgm->SetLoop(false);
			bgm->Play();
			stageTime -= 1.0f;
			stringstream ss;
			ss << "TIME-" << Utils::ToString((int)stageTime, "0000");
			timeText->SetText(ss.str());
			IncreaseScore(100.0f);
		}
	}

	if (!bgm->IsPlaying())
	{
		FRAMEWORK.SetTimeScale(1.0f);
		SpriteTextGO* sysMsgText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsg");
		RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsgRect");
		sysMsgText->SetActive(false);
		sysMsgRect->SetActive(false);
		GetHurdleManager().ReturnAll();
		hurdleManager.RemoveObj();
		IncreaseStage();
		Penta* player = (Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player");
		player->Reset();
		state = StageState::Playing;
		SetSpeedLevel(4);
		iceStation->Reset();
		iceStation->SetActive(false);
		bgm->SetClip(Resources.GetSoundBuffer("sound/bg/2_MainBgm.ogg"));
		bgm->SetLoop(false);
		bgm->Play();
		updateFunc = std::bind(&StageManager::UpdatePlaying, this, std::placeholders::_1);
	}
}

void StageManager::Draw(sf::RenderWindow& window)
{
	hurdleManager.Draw(window);
	iceStation->Draw(window);
}

void StageManager::OnGUI(sf::RenderWindow& window)
{
	hurdleManager.OnGUI(window);
}

void StageManager::IncreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(++speedLevel, 0, speedLevelMax);

	background->SetSpeed(GetSpeed());
	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "speed ";
	for (int i = 0; i < speedLevel; i++)
	{
		ss << '|';
	}
	speedText->SetText(ss.str());
}


void StageManager::DecreaseSpeedLevel()
{
	speedLevel = Utils::Clamp(--speedLevel, 1, speedLevelMax);
	background->SetSpeed(GetSpeed());

	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "speed ";
	for (int i = 0; i < speedLevel; i++)
	{
		ss << '|';
	}
	speedText->SetText(ss.str());
}

void StageManager::SetSpeedLevel(const int& level)
{
	speedLevel = Utils::Clamp(level, 0, speedLevelMax);
	background->SetSpeed(GetSpeed());

	SpriteTextGO* speedText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SpeedText");
	stringstream ss;
	ss << "speed ";
	for (int i = 0; i < speedLevel; i++)
	{
		ss << '|';
	}
	speedText->SetText(ss.str());
}

void StageManager::IncreaseScore(const int& score)
{
	this->score += score;

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "1P-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	PlayerPrefs.SetInt("HighScore", max(score, PlayerPrefs.GetInt("HighScore", 0)));

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}

void StageManager::IncreaseScore(const ScoreItemType& type)
{
	this->score += scoreTable[(int)type];

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "1P-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	PlayerPrefs.SetInt("HighScore", max(score, PlayerPrefs.GetInt("HighScore", 0)));

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}

void StageManager::ResetScore()
{
	this->score = 0;

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "1P-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());

	SpriteTextGO* highScoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("HighScoreText");
	ss.str("");
	ss << "HI-" << Utils::ToString(PlayerPrefs.GetInt("HighScore", 0), "000000");
	highScoreText->SetText(ss.str());
}

void StageManager::IncreaseStage()
{
	stage++;

	SpriteTextGO* stageText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("StageText");
	stringstream ss;
	ss << "STAGE-" << Utils::ToString(this->stage, "00");
	const StageDataTable* stageDataTable = DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage);
	stageTime = stageDataTable->GetTimeLimit(stage - 1);
	stageRest = stageDataTable->GetDistance(stage - 1);
	stageText->SetText(ss.str());

	stageName = DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage)->GetStageName(stage - 1);
	background->SetSkyColor(DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage)->GetSkyColor(stage - 1));
}

void StageManager::SetStage(const int& stage)
{
	this->stage = stage;

	SpriteTextGO* stageText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("StageText");
	stringstream ss;
	ss << "STAGE-" << Utils::ToString(this->stage, "00");
	const StageDataTable* stageDataTable = DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage);
	stageTime = stageDataTable->GetTimeLimit(stage - 1);
	stageRest = stageDataTable->GetDistance(stage - 1);
	stageText->SetText(ss.str());

	stageName = DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage)->GetStageName(stage - 1);
	background->SetSkyColor(DATATABLE_MANAGER.Get<StageDataTable>(DataTable::Ids::Stage)->GetSkyColor(stage - 1));
}

const std::string& StageManager::GetStageName() const
{
	return stageName;
}

void StageManager::StageClear()
{
	state = StageState::Clear;

	bgm->SetClip(Resources.GetSoundBuffer("sound/bg/3_StageClear.ogg"));
	bgm->SetLoop(false);
	bgm->Play();

	SpriteTextGO* sysMsgText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsg");
	sysMsgText->SetText(stageName);
	sysMsgText->SetPosition(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.3f);
	sysMsgText->SetOrigin(Origins::MC);
	sysMsgText->SetActive(true);

	RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsgRect");
	sysMsgRect->SetSize(sysMsgText->GetSize());
	sysMsgRect->SetPosition(sysMsgText->GetPosition());
	sysMsgRect->SetOrigin(Origins::MC);
	sysMsgRect->SetActive(true);

	speedLevel = 0;
	background->SetSpeed(0.0f);

	Penta* player = (Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player");
	player->Clear();
	effectTime = 6.0f;
	updateFunc = std::bind(&StageManager::UpdateClear, this, std::placeholders::_1);
}