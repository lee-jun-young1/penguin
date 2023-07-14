#pragma once
#include "Collider.h"
class BoxCollider : public Collider
{
protected:
	sf::Rect<float> rect;
public:
	BoxCollider(GameObject& gameObject);

	void SetRect(sf::Rect<float> rect);
	sf::Rect<float> GetRect();

	virtual bool CheckCross(Collider* col) override;

	virtual void Update(float deltaTime) override;

	// Collider을(를) 통해 상속됨
	virtual sf::Vector2f GetCenter() override;
	virtual float GetWidth() override;
	virtual float GetHeight() override;
};

