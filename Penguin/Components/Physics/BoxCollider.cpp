#include "stdafx.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "GameObject.h"

BoxCollider::BoxCollider(GameObject& gameObject)
	:Collider(gameObject, ColliderType::Box)
{
}

void BoxCollider::SetRect(sf::Rect<float> rect)
{
	this->rect = rect;
}

sf::Rect<float> BoxCollider::GetRect()
{
	return rect;
}

bool BoxCollider::CheckCross(Collider* col)
{
	switch (col->GetColliderType())
	{
	case ColliderType::Box:
	{
		return rect.intersects(((BoxCollider*)col)->GetRect());
	}
	case ColliderType::Circle:
	{
		//Âü°í : https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
		CircleCollider* circleCol = (CircleCollider*)col;
		sf::Vector2f circleCenter = circleCol->GetCenter();
		float radius = circleCol->GetRaidus();
		sf::Vector2f boxCenter = GetCenter();

		float xDistance = abs(circleCenter.x - boxCenter.x);
		float yDistance = abs(circleCenter.y - boxCenter.y);

		//³ì»ö¿µ¿ª
		if (xDistance > (rect.width * 0.5f + radius)) { return false; }
		if (yDistance > (rect.height * 0.5f + radius)) { return false; }

		//Çª¸¥»ö¿µ¿ª
		if (xDistance <= (rect.width * 0.5f)) { return true; }
		if (yDistance <= (rect.height * 0.5f)) { return true; }

		//»¡°£¿µ¿ª
		float cornerDistance = (xDistance - rect.width * 0.5f) * (xDistance - rect.width * 0.5f)
			+ (yDistance - rect.height * 0.5f) * (yDistance - rect.height * 0.5f);
		return (cornerDistance <= (radius * radius));
	}
	}
}

void BoxCollider::Update(float deltaTime)
{
	rect.left = gameObject.GetPosition().x + offset.x;
	rect.top = gameObject.GetPosition().y + offset.y;
	Collider::Update(deltaTime);
}

sf::Vector2f BoxCollider::GetCenter()
{
	return sf::Vector2f((rect.left + rect.width * 0.5f), (rect.top + rect.height * 0.5f));
}

float BoxCollider::GetWidth()
{
	return rect.width;
}

float BoxCollider::GetHeight()
{
	return rect.height;
}
