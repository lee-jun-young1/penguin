#pragma once
#include "GameObject.h"
using namespace std;

class TextGameObj : public GameObject
{
protected:
	string fontID;
	int fontSize;

public:
	sf::Text text;

	TextGameObj(const string& fontID, const int& fontSize, const string& name);
	virtual ~TextGameObj();

	sf::Text& GetText();
	void SetPosition(float width, float height);
	void SetOrigin(Origins origin);

	void SetFont(sf::Font& font);
	void SetCharacterSize(const int& fontSize);
	void SetString(const string& str);
	void SetFillColor(const sf::Color& color);
	
	// GameObject��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

