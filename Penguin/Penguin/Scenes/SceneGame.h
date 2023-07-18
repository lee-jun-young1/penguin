#pragma once
#include "Scene.h"
#include <ObstacleManager.h>
class SceneGame : public Scene
{
protected:
	std::map<std::string, bool> mouseOverStates;

	ObstacleManager* obstacleManager;

public:
	SceneGame();
	virtual ~SceneGame() override;

	virtual void Enter() override;
	void Reset();
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;

	ObstacleManager* GetObstacleManager();
};

