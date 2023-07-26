#pragma once
#include "SpriteGO.h"
#include <Penta.h>
class PentaShadow : public SpriteGO
{
private:
	Penta* penta;
	const float yPos = 165.0f;
	const float yPosInverse = 1.0f / 165.0f;
public:
	PentaShadow(const std::string textureID = "graphics/Penta_Shadow.png", const std::string& name = "PentaShadow");
	virtual void Update(float dt) override;
	void SetPenta(Penta* penta) { this->penta = penta; }
};

