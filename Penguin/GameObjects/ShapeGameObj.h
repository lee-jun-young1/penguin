#pragma once
#include "GameObject.h"
#include <SFML/Graphics.hpp>

class ShapeGameObj: public GameObject
{
protected:
	sf::Shape* shape;
	ShapeType type;
public:
	ShapeGameObj(ShapeType shapeType, const string& name = "");

	// GameObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetOrigin(Origins origin);
	virtual void SetPosition(const sf::Vector2f& position) override;
	void SetFillColor(sf::Color color);


	// GameObject��(��) ���� ��ӵ�
	virtual void Reset() override;

};

