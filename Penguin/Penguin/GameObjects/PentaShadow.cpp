#include "stdafx.h"
#include "PentaShadow.h"

PentaShadow::PentaShadow(const std::string textureID, const std::string& name)
	:SpriteGO(textureID, name)
{
}

void PentaShadow::Update(float dt)
{
	SpriteGO::Update(dt);
	SetPosition(penta->GetPosition().x, penta->GetState() == PentaState::Clear ? penta->GetPosition().y : yPos);
	sprite.setScale({ penta->GetPosition().y * yPosInverse, 1.0f });
	SetOrigin(Origins::MC);
}
