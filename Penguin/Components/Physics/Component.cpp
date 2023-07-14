#include "stdafx.h"
#include "Component.h"
#include <GameObject.h>

bool Component::IsUnique()
{
	return isUnique;
}

ComponentType Component::GetComponentType()
{
	return componentType;
}

Component::Component(GameObject& gameObject, ComponentType type, bool isUnique)
	:gameObject(gameObject), componentType(type), isUnique(isUnique)
{
}
