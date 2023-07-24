#include "stdafx.h"
#include "SceneTitle.h"
#include "SpriteGO.h"
#include <Framework.h>
#include <Components/Audio/AudioSource.h>
#include <InputManager.h>
#include <SceneManager.h>
#include <GameObjects/SpriteTextGO.h>

SceneTitle::SceneTitle()
	:Scene(SceneId::Title)
{
	sceneName = "SceneTitle";
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	auto screenCenter = size * 0.5f;
	uiView.setSize(size);
	uiView.setCenter(screenCenter.x, screenCenter.y);
	worldView.setSize(size);
	worldView.setCenter(screenCenter);


	Scene::Enter();

	SpriteFont* font = new SpriteFont("fonts/SpriteFont_Data.csv");

	SpriteTextGO* originKonami = (SpriteTextGO*)FindGameObject("OriginKonami");
	originKonami->sortLayer = UILayer;
	originKonami->SetFont(font);
	originKonami->SetText("ORIGIN  BY  @ KONAMI  1984");
	originKonami->SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f , FRAMEWORK.GetWindowSize().y * 0.25f });
	originKonami->SetOrigin(Origins::MC);

	SpriteTextGO* playSelect = (SpriteTextGO*)FindGameObject("PlaySelect");
	playSelect->sortLayer = UILayer;
	playSelect->SetFont(font);
	playSelect->SetText("PLAY SELECT");
	playSelect->SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f , FRAMEWORK.GetWindowSize().y * 0.45f });
	playSelect->SetOrigin(Origins::MC);

	SpriteTextGO* press1Key = (SpriteTextGO*)FindGameObject("Press1Key");
	press1Key->sortLayer = UILayer;
	press1Key->SetFont(font);
	press1Key->SetText("1-k#$    PLAY %  JOYSTICK", 1);
	press1Key->SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f , FRAMEWORK.GetWindowSize().y * 0.55f });
	press1Key->SetOrigin(Origins::MC);

	SpriteTextGO* press2Key = (SpriteTextGO*)FindGameObject("Press2Key");
	press2Key->sortLayer = UILayer;
	press2Key->SetFont(font);
	press2Key->SetText("2-k#$    PLAY %  KEYBOARD", 1);
	press2Key->SetPosition({ FRAMEWORK.GetWindowSize().x * 0.5f , FRAMEWORK.GetWindowSize().y * 0.6f });
	press2Key->SetOrigin(Origins::MC);

	Reset();
}

void SceneTitle::Reset()
{
	for (auto go : gameObjects)
	{
		go->Reset();
	}
	AudioSource* bgm = (AudioSource*)FindGameObject("Background")->GetComponent(ComponentType::Audio);
	bgm->SetClip(Resources.GetSoundBuffer("sound/bg/1_Opening.ogg"));
	bgm->SetLoop(true);
	bgm->Play();

}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Init()
{
	Scene::Init();
	Release();

	SpriteGO* bg = new SpriteGO("graphics/Title.png", "Background");
	bg->sortLayer = -1;

	AudioSource* bgm = new AudioSource(*bg);
	bgm->SetVolume(100.0f);
	bg->AddComponent(bgm);

	AddGameObject(bg);

	SpriteTextGO* originKonami = new SpriteTextGO("OriginKonami");
	AddGameObject(originKonami);
	SpriteTextGO* playSelect = new SpriteTextGO("PlaySelect");
	AddGameObject(playSelect);
	SpriteTextGO* press1Key = new SpriteTextGO("Press1Key");
	AddGameObject(press1Key);
	SpriteTextGO* press2Key = new SpriteTextGO("Press2Key");
	AddGameObject(press2Key);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{
	for (auto go : gameObjects)
	{
		delete go;
	}
}

void SceneTitle::Update(float deltaTime)
{
	Scene::Update(deltaTime);
	if (Input.GetKeyDown(sf::Keyboard::Num2))
	{
		SCENE_MANAGER.ChangeScene(SceneId::Game);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
