#pragma once
#include "Scene.h"
class SceneGame : public Scene
{
protected:
public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

