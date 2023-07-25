#pragma once
#include "Scene.h"
#include <StageManager.h>
#include "SceneGame.h"
class SceneDemo : public SceneGame
{
protected:
	std::map<std::string, bool> mouseOverStates;

	StageManager* stageManager;

	float demoTime = 30.0f;
public:
	SceneDemo();
	virtual ~SceneDemo() override;

	virtual void Enter() override;
	virtual void Reset() override;
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;

	StageManager* GetStageManager();
};

