#pragma once
#include "Penta.h"
class PentaBot : public Penta
{
protected:
	float actionCooldown = 0.0f;
public:
	PentaBot(const std::string& textureID = "", const std::string& name = "")
		:Penta(textureID, name) {}	
	
	
	virtual void Reset() override;

	virtual void Update(float dt) override;

	virtual void UpdateHit(float deltaTime) override;
	virtual void UpdateCrevasse(float deltaTime) override;
	virtual void UpdateMove(float deltaTime) override;
	virtual void UpdateClear(float deltaTime) override;
	virtual void UpdateJump(float deltaTime) override;
	virtual void UpdatePegicopter(float deltaTime) override;
	virtual void OnCollisionEnter(Collider* col) override;
};

