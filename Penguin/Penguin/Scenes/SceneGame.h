#pragma once
#include "Scene.h"
#include <StageManager.h>
class SceneGame : public Scene
{
protected:
	std::map<std::string, bool> mouseOverStates;

	StageManager* stageManager;

public:
	SceneGame();
	virtual ~SceneGame() override;

	virtual void Enter() override;
	virtual void Reset() override;
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;

	StageManager* GetStageManager();
};

