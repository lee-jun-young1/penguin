#pragma once
#include <random>
#include <Defines.h>
#include <SFML/Graphics.hpp>

class Utils
{
private:
	static std::random_device randomDevice;
	static std::mt19937 gen;
public:
	/// <returns>
	/// min�̻� max�̸��� ������ ���� ����ϴ�.
	/// </returns>
	static int RandomRange(int min, int maxExclude);
	/// <returns>
	/// min�̻� max������ ������ ���� ����ϴ�.
	/// </returns>
	static float RandomRange(float min, float max);	
	
	static sf::Vector2f RandomOnCircle(float radius);
	static sf::Vector2f RandomInCircle(float radius);
	static float RandomValue();

	static void SetOrigin(sf::Sprite& sprite, Origins origin);
	static void SetOrigin(sf::Text& text, Origins origin);
	static void SetOrigin(sf::Shape& shape, Origins origin);
	static void SetOrigin(sf::Transformable& transformable, Origins origin, const sf::FloatRect & rect);

	static float Clamp(float v, float min, float max);
	static int Clamp(int v, int min, int max);
	//static int Clamp(int v, int min, int max);
	static sf::Vector2f Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max);

	static float Lerp(float a, float b, float t, bool clamping = true);
	static sf::Vector2f Lerp(sf::Vector2f a, sf::Vector2f b, float t, bool clamping = true);
	static sf::Color Lerp(sf::Color a, sf::Color b, float t, bool clamping = true);

	static const sf::Vector2f Normalize(const sf::Vector2f& vector);
	static float Magnitude(const sf::Vector2f& vector2);
	static float SqrMagnitude(const sf::Vector2f& vector);

	static float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

	static float Angle(const sf::Vector2f& start, const sf::Vector2f& end);
	static float Angle(const sf::Vector2f&direction);

	static sf::Vector2f RectNormal(sf::Rect<float> base, sf::Rect<float> target);
	static std::string ToString(int value, std::string format);
	static std::string ToString(float value, std::string format);
	static std::vector<std::string> Split(std::string input, char delimiter);
	static bool Contains(const std::string& str, const char& c);
};