#pragma once
#include "stdafx.h"
#include "Component.h"
#include "Collider.h"
using namespace std;

class GameObject
{
protected:
	string name;
	sf::Vector2f position;
	bool isActive = true;//true : Run Update(), Draw()
	Origins origin;
	list<Component*> components;
public:
	int sortLayer = 0;
	int sortOrder = 0;
	int physicsLayer = 0;

	bool IsActive() const;
	void SetActive(bool active);

	string GetName();
	virtual void SetName(const std::string& name);

	sf::Vector2f GetPosition();
	virtual void SetPosition(const sf::Vector2f& position);

	virtual void SetPosition(const float& x, const float& y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float originX, float originY);

	virtual void Init() = 0;
	virtual void Release() {};

	virtual void Reset() = 0; //√ ±‚»≠

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	GameObject(const std::string& name);
	virtual ~GameObject();

	template <typename T>
	T* GetComponent();
	template <typename T>
	void GetComponents(std::list<T*>& list);


	Component* GetComponent(const ComponentType type);
	void GetComponents(std::list<Component*>& list, const ComponentType type);
	bool Exist(Component* component);
	void AddComponent(Component* component);
	void RemoveGameObject(Component* component);



	virtual void OnCollisionEnter(Collider* col) {};
	virtual void OnCollisionStay(Collider* col) {};
	virtual void OnCollisionExit(Collider* col) {};
	virtual void OnTriggerEnter(Collider* col) {};
	virtual void OnTriggerStay(Collider* col) {};
	virtual void OnTriggerExit(Collider* col) {};
};

template<typename T>
inline T* GameObject::GetComponent()
{
	auto it = components.begin();
	while (it != components.end())
	{
		if (dynamic_cast<T>(*it) != nullptr)
		{
			return *it;
		}
	}
	return nullptr;
}

template<typename T>
inline void GameObject::GetComponents(std::list<T*>& list)
{
	auto it = components.begin();
	while (it != components.end())
	{
		if (dynamic_cast<T>(*it) != nullptr)
		{
			list.push_back(*it);
		}
	}
}
