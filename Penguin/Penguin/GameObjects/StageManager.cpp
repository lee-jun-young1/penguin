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
void StageManager::Init()
{
	crevassePool.OnCreate = [this](Crevasse* crevasse) { crevasse->SetManager(this); };
	iceHolePool.OnCreate = [this](IceHole* iceHole) 
	{
		iceHole->SetManager(this);
		int random = Utils::RandomRange(0, 4);
		switch(random)
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
	fishPool.OnCreate = [this](Fish* fish) { fish->SetManager(this); };
	flagPool.OnCreate = [this](FlagItem* flag) { flag->SetManager(this); };
	crevassePool.Init(20);
	iceHolePool.Init(20);
	fishPool.Init(20);
	flagPool.Init(20);

	iceStation = new IceStation();
	iceStation->SetManager(this);
	iceStation->SetDirection({ FRAMEWORK.GetWindowSize().x * 0.5f, startY}, { FRAMEWORK.GetWindowSize().x * 0.5f, endY - 50.0f});
	iceStation->Init();
	manageObjects.push_back(iceStation);

	timeLimitAlert = new AudioSource(*this);
	timeLimitAlert->SetLoop(false);
	AddComponent(timeLimitAlert);
}

void StageManager::Release()
{
	crevassePool.Release();
	iceHolePool.Release();
	fishPool.Release();
	flagPool.Release();
}

void StageManager::Reset()
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
	

	SetSpeedLevel(4);

	updateFunc = std::bind(&StageManager::UpdatePlaying, this, std::placeholders::_1);
}

void StageManager::Update(float dt)
{
	updateFunc(dt);
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
		CreateObj();
	}
	else if (stageRest <= 50.0f)
	{
		iceStation->SetActive(true);
	}


	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->Update(refreshTime);
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
	for (auto fish : fishPool.GetUseList())
	{
		if (!fish->IsActive())
		{
			fishPool.Return(fish);
		}
	}
	for (auto flag : flagPool.GetUseList())
	{
		if (!flag->IsActive())
		{
			flagPool.Return(flag);
		}
	}
	RemoveObj();

	refreshTime = 0;
}

void StageManager::CreateObj()
{
	int random = Utils::RandomRange(0, 4);

#ifdef _DEBUG
	if (Input.GetKey(sf::Keyboard::F5))
	{
		random = 1;
	}
	if (Input.GetKey(sf::Keyboard::F6))
	{
		random = 2;
	}
	if (Input.GetKey(sf::Keyboard::F7))
	{
		random = 3;
	}
#endif
	switch (random)
	{
	case 0:
		break;
	case 1:
	{
		Crevasse* crevasse = &GetCrevasse();
		float t = Utils::RandomValue();
		crevasse->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(crevasse);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(crevasse);
	}
	break;
	case 2:
	{
		IceHole* iceHole = &GetIceHole();
		float t = Utils::RandomValue();
		iceHole->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(iceHole);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(iceHole);

		Seal* seal = iceHole->GetSeal();
		if (seal != nullptr)
		{
			seal->SetActive(true);
			manageObjects.push_back(seal);
			//SCENE_MANAGER.GetCurrentScene()->AddGameObject(seal);
			seal->Reset();
		}
	}
	break;
	case 3:
	{
		FlagItem* flag = &GetFlag();
		float t = Utils::RandomValue();
		flag->SetDirection({ Utils::Lerp(startXRange.x, startXRange.y, t), startY }, { Utils::Lerp(endXRange.x, endXRange.y, t), endY });
		manageObjects.push_back(flag);
		//SCENE_MANAGER.GetCurrentScene()->AddGameObject(flag);
	}
	break;
	}
}

void StageManager::UpdateTimeOut(float dt)
{
	if (!bgm->IsPlaying())
	{
		FRAMEWORK.SetTimeScale(1.0f);
		SpriteTextGO* sysMsgText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsg");
		RectangleShapeGO* sysMsgRect = (RectangleShapeGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("SysMsgRect");
		sysMsgText->SetActive(false);
		sysMsgRect->SetActive(false);
		ReturnAll();
		RemoveObj();
		SCENE_MANAGER.ChangeScene(SceneId::Title);
	}
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
		ReturnAll();
		RemoveObj();
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

void StageManager::RemoveObj()
{
	for (auto removeObj : removeManageObjects)
	{
		manageObjects.remove(removeObj);
	}
	removeManageObjects.clear();
}

void StageManager::Draw(sf::RenderWindow& window)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->Draw(window);
		}
	}
}

void StageManager::OnGUI(sf::RenderWindow& window)
{
	for (auto& obj : manageObjects)
	{
		if (obj->IsActive())
		{
			obj->OnGUI(window);
		}
	}
}

Crevasse& StageManager::GetCrevasse()
{
	cout << "crevassePool get :: " << crevassePool.GetUseList().size() + 1 << endl;
	return *crevassePool.Get();
}

void StageManager::ReturnCrevasse(Crevasse* crevasse)
{
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(crevasse);
	RemoveManageObject(crevasse);
	crevassePool.Return(crevasse);
	cout << "crevassePool return :: " << crevassePool.GetUseList().size() << endl;
}

Fish& StageManager::GetFish()
{
 	return *fishPool.Get();
}

void StageManager::ReturnFish(Fish* fish)
{
	SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(fish);
	fishPool.Return(fish);
}

IceHole& StageManager::GetIceHole()
{
	return *iceHolePool.Get();
}

void StageManager::ReturnIceHole(IceHole* iceHole)
{
	if (iceHole->GetSeal() != nullptr)
	{
		RemoveManageObject(iceHole->GetSeal());
		//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole->GetSeal());
	}
	RemoveManageObject(iceHole);
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(iceHole);
	iceHolePool.Return(iceHole);
}

FlagItem& StageManager::GetFlag()
{
	return *flagPool.Get();
}

void StageManager::ReturnFlag(FlagItem* flag)
{
	//SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(flag);
	RemoveManageObject(flag);
	flagPool.Return(flag);
}

void StageManager::ReturnAll()
{

	for (auto& crevasse : crevassePool.GetUseList())
	{
		crevasse->Reset();
		RemoveManageObject(crevasse);
	}
	for (auto& iceHole : iceHolePool.GetUseList())
	{
		if (iceHole->GetSeal() != nullptr)
		{
			iceHole->GetSeal()->Reset();
			RemoveManageObject(iceHole->GetSeal());
		}
		iceHole->Reset();
		RemoveManageObject(iceHole);
	}
	for (auto& fish : fishPool.GetUseList())
	{
		fish->Reset();
		SCENE_MANAGER.GetCurrentScene()->RemoveGameObject(fish);
	}
	for (auto& flag : flagPool.GetUseList())
	{
		flag->Reset();
		RemoveManageObject(flag);
	}

	crevassePool.Clear();
	iceHolePool.Clear();
	fishPool.Clear();
	flagPool.Clear();
}

void StageManager::RemoveManageObject(GameObject* go)
{
	removeManageObjects.push_back(go);
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
}

void StageManager::IncreaseScore(const ScoreItemType& type)
{
	this->score += scoreTable[(int)type];

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "1P-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());
}

void StageManager::ResetScore()
{
	this->score = 0;

	SpriteTextGO* scoreText = (SpriteTextGO*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("ScoreText");
	stringstream ss;
	ss << "1P-" << Utils::ToString(this->score, "000000");
	scoreText->SetText(ss.str());
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

	Penta* player = (Penta*)SCENE_MANAGER.GetCurrentScene()->FindGameObject("Player");
	player->Clear();
	effectTime = 6.0f;
	updateFunc = std::bind(&StageManager::UpdateClear, this, std::placeholders::_1);
}