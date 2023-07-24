#pragma once
#include "Scene.h"
class SceneTitle : public Scene
{
protected:

public:
	SceneTitle();
	virtual ~SceneTitle() override;

	virtual void Enter() override;
	virtual void Reset() override;
	virtual void Exit() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

