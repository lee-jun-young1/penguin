#pragma once
#include "GameObject.h"
#include "SFML/Graphics.hpp"
#include "ResourceManager.h"

class SpriteGO : public GameObject
{
protected:
	sf::Vector2f spriteDirection;

public:
	sf::Sprite sprite;
	std::string textureID;
	SpriteGO(const std::string textureID = "", const std::string & name = "");

	void SetTexture(sf::Texture& texture, sf::Vector2f spriteDir = sf::Vector2f(-1.0f, 0.0f));
	void SetTextureID(const std::string textureID);

	virtual void SetPosition(const sf::Vector2f& position) override;
	virtual void SetPosition(const float& x, const float& y) override;

	void SetOrigin(Origins origin) override;
	void SetOrigin(float originX, float originY) override;
	sf::Vector2f GetOrigin() const;

	sf::Vector2f GetSize() const;

	// GameObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetFlipX(bool flip);
	void SetFlipY(bool flip);
};

