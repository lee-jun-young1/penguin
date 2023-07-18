#pragma once
#include "GameObject.h"
#include "ObjectPool.h"
#include <Crevasse.h>
#include <IceHole.h>
#include <Utils.h>
#include <Framework.h>
class ObstacleManager : public GameObject
{
private:
	ObjectPool<Crevasse> crevassePool;
	ObjectPool<IceHole> iceHolePool;

	float cycle = 1.0f;
	float time = 0.0f;

	float speed = 0.2f;
	int speedLevelMax = 5;
	int speedLevel = 1;

	sf::Vector2f startXRange = { FRAMEWORK.GetWindowSize().x * 0.4f, FRAMEWORK.GetWindowSize().x * 0.6f };
	float startY = 60.0f;
	sf::Vector2f endXRange = { FRAMEWORK.GetWindowSize().x * 0.2f, FRAMEWORK.GetWindowSize().x * 0.8f };
	float endY = 180.0f;
public:

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override; //√ ±‚»≠

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;

	Crevasse& GetCrevasse();
	void ReturnCrevasse(Crevasse* crevasse);
	IceHole& GetIceHole();
	void ReturnIceHole(IceHole* iceHole);

	void ReturnAll();

	float GetSpeed() { return speed * speedLevel; };
	void SetSpeedLevel(int i) { speedLevel = Utils::Clamp(speedLevel, 0, speedLevelMax); };
};

