#pragma once
class GameObject;

class Component
{
protected:
	GameObject& gameObject;
	ComponentType componentType;
	bool isUnique;
public:
	bool IsUnique();
	ComponentType GetComponentType();

	GameObject& GetGameObject();

	Component(GameObject& gameObject, ComponentType type, bool isUnique);

	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void Release() = 0;
};

