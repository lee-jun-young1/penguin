#pragma once
#include "SpriteGO.h"
#include <BoxCollider.h>
class StageManager;

class FlagItem : public SpriteGO
{
protected:
    ScoreItemType itemType = ScoreItemType::Flag;

    StageManager* manager;

    BoxCollider* collider;

    const sf::Vector2f sizeMin = { 1.0f, 1.0f };
    const sf::Vector2f sizeMax = { 24.0f, 20.0f };
    sf::Vector2f size;

    sf::Vector2f startPos;
    sf::Vector2f endPos;

    float time = 0.0f;
public:
    FlagItem(const std::string textureID = "graphics/Flag.png", const std::string& name = "Flag") : SpriteGO(textureID, name) {}
    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float deltaTime) override;
    void OnTriggerEnter(Collider* col);
    void SetManager(StageManager* manager);

    virtual void SetSize(sf::Vector2f size);

    void SetDirection(sf::Vector2f startPos, sf::Vector2f endPos);
};

