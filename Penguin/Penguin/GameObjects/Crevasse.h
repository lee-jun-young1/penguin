#pragma once
#include "SlicedSpriteGO.h"
#include "BoxCollider.h"
#include "CrevasseCenter.h"
class ObstacleManager;

class Crevasse : public SlicedSpriteGO
{
protected:
	ObstacleManager* manager;

	const sf::Vector2f sizeMin = { 60.0f, 0.0f };
	const sf::Vector2f sizeMax = { 160.0f, 16.0f };
	sf::Vector2f size;
	CrevasseCenter* center;
	GameObject* side; 

	BoxCollider* centerCol;
	BoxCollider* leftCol;
	BoxCollider* rightCol;

	sf::Vector2f startPos;
	sf::Vector2f endPos;
	float speed = 0.2f;
	float time = 0.0f;
public:
	Crevasse(const std::string& textureID = "graphics/Crevasse.png", sf::Rect<float> centerRect = { 32, 2, 96, 13 }, sf::Rect<float> size = { 0, 0, 160, 16 });
	void SetManager(ObstacleManager* manager);
	// GameObject을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void OnGUI(sf::RenderWindow& window) override;


	void SetDirection(sf::Vector2f startPos, sf::Vector2f endPos);

	virtual void OnTriggerEnter(Collider* col) override;
	virtual void OnTriggerStay(Collider* col) override;
	virtual void OnTriggerExit(Collider* col) override;
	void SetSize(sf::Vector2f size);
};

