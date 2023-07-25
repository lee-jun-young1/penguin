#pragma once
#include "SpriteGO.h"
#include <BoxCollider.h>
#include <Framework.h>
class StageManager;

class IceStation : public SpriteGO
{
protected:
    StageManager* manager;
    SpriteGO* flag;

    const sf::Vector2f sizeMin = { 0.0f, 0.0f };
    const sf::Vector2f sizeMax = { 1.0, 1.0f };
    sf::Vector2f size;

    sf::Vector2f startPos;
    sf::Vector2f endPos;


    sf::Vector2f flagStartLocalPos;
    sf::Vector2f flagEndLocalPos;

    float time = 0.0f;
public:
    IceStation(const std::string textureID = "graphics/IceStation.png", const std::string& name = "IceStation") : SpriteGO(textureID, name) {}
    virtual void Init() override;
    virtual void Reset() override;
    virtual void Update(float deltaTime) override;
    virtual void Draw(sf::RenderWindow& window) override;
    void SetManager(StageManager* manager);

    virtual void SetSize(sf::Vector2f size);

    void SetDirection(sf::Vector2f startPos, sf::Vector2f endPos);
};

