#pragma once
#include "GameObject.h"
class CrevasseCenter : public GameObject
{
public:
	CrevasseCenter(const string& name) :GameObject(name){}
	virtual void OnTriggerExit(Collider* col) override { parent->OnTriggerExit(col); }
};

