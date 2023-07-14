#pragma once
#include "Collider.h"
class CircleCollider : public Collider
{
protected:
	sf::Vector2f center;
	float radius;
public:
	CircleCollider(GameObject& gameObject);

	void SetRadius(const float& radius);
	float GetRaidus();

	virtual void Update(float deltaTime) override;

	virtual bool CheckCross(Collider* col) override;

	// Collider��(��) ���� ��ӵ�
	virtual sf::Vector2f GetCenter() override;
	virtual float GetWidth() override;
	virtual float GetHeight() override;
};

