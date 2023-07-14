#pragma once
#include "Singleton.h"
class Framework : public Singleton<Framework>
{
	friend Singleton<Framework>;
protected:
	Framework() = default;
	virtual ~Framework() override = default;

	sf::RenderWindow window;
	sf::Clock clock;

	int screenWidth = 1280;
	int screenHeight = 720;
	std::string title = "Antarctic Adventure!";

	float gravity = 9.8f;
	float dotPerMeter = 100.0f;

	sf::Cursor cursor;

	bool isGameExitButton = false;
	
public:
	Framework(int width, int height, const std::string& title);

	virtual void Init(int width, int height, const std::string& title);
	virtual void Release();

	virtual void Update(float deltaTime);
	virtual void Draw();

	virtual void Run();

	void SetCursor(const sf::Uint8* pixels, sf::Vector2u size, sf::Vector2u hotspot);

	sf::Vector2f GetWindowSize();
	sf::RenderWindow& GetWindow();
	void ExitButton(bool exit);

	float GetGravity();
	float GetDPM();

};

#define FRAMEWORK (Framework::GetInstance()) 