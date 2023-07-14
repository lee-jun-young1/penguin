#include "stdafx.h"
#include "Utils.h"
#include <SFML/Graphics.hpp>

std::random_device Utils::randomDevice;
std::mt19937 Utils::gen(Utils::randomDevice());

int Utils::RandomRange(int min, int maxExclude)
{
	std::uniform_int_distribution<int> dist(min, maxExclude - 1);
	return dist(gen);
}

float Utils::RandomRange(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return (float)dist(gen);
}

sf::Vector2f Utils::RandomOnCircle(float radius)
{
	sf::Vector2f result(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f));
	result = Normalize(result) * radius;
	return result;
}

sf::Vector2f Utils::RandomInCircle(float radius)
{
	return RandomOnCircle(radius) * RandomValue();
}

float Utils::RandomValue()
{
	return RandomRange(0.0f, 1.0f);
}

void Utils::SetOrigin(sf::Sprite& sprite, Origins origin)
{
	SetOrigin(sprite, origin, sprite.getLocalBounds());
}

void Utils::SetOrigin(sf::Text& text, Origins origin)
{
	SetOrigin(text, origin, text.getLocalBounds());
}

void Utils::SetOrigin(sf::Shape& shape, Origins origin)
{
	SetOrigin(shape, origin, shape.getLocalBounds());
}

void Utils::SetOrigin(sf::Transformable& transformable, Origins origin, const sf::FloatRect& rect)
{
	sf::Vector2f originPos(rect.width, rect.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	transformable.setOrigin(originPos);
}

float Utils::Clamp(float v, float min, float max)
{
	return std::max(std::min(v, max), min);
}

sf::Vector2f Utils::Clamp(const sf::Vector2f& v, const sf::Vector2f& min, const sf::Vector2f& max)
{
	sf::Vector2f result;
	result.x = Clamp(v.x, min.x, max.x);
	result.y = Clamp(v.y, min.y, max.y);
	return result;
}

float Utils::Lerp(float a, float b, float t, bool clamping)
{
	if (clamping)
	{
		t = Clamp(t, 0.0f, 1.0f);
	}
	return a + (b - a) * t;
}

sf::Vector2f Utils::Lerp(sf::Vector2f a, sf::Vector2f b, float t, bool clamping)
{
	if (clamping)
	{
		t = Clamp(t, 0.0f, 1.0f);
	}
	return a + (b - a) * t;
}

sf::Color Utils::Lerp(sf::Color a, sf::Color b, float t, bool clamping)
{
	sf::Color result;
	if (clamping)
	{
		t = Clamp(t, 0.0f, 1.0f);
	}
	result.r = a.r + (b.r - a.r) * t;
	result.g = a.g + (b.g - a.g) * t;
	result.b = a.b + (b.b - a.b) * t;
	result.a = a.a + (b.a - a.a) * t;
	return result;
}

const sf::Vector2f Utils::Normalize(const sf::Vector2f& vector)
{
	float mag = Magnitude(vector);
	if (mag == 0.f)
	{
		return vector;
	}
	return vector / mag;
}

float Utils::Magnitude(const sf::Vector2f& vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y);
}

float Utils::SqrMagnitude(const sf::Vector2f& vector)
{
	return vector.x * vector.x + vector.y * vector.y;
}

float Utils::Distance(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	return Magnitude(p2 - p1);
}



float Utils::Angle(const sf::Vector2f& start, const sf::Vector2f& end)
{
	return Angle(Normalize(end - start));
}

float Utils::Angle(const sf::Vector2f& direction)
{
	return atan2(direction.y, direction.x) * (180.0f / M_PI);
}

sf::Vector2f Utils::RectNormal(sf::Rect<float> base, sf::Rect<float> target)
{
	sf::Vector2f baseCenter(base.left + base.width * 0.5f, base.top + base.height * 0.5f);
	sf::Vector2f targetCenter(target.left + target.width * 0.5f, target.top + target.height * 0.5f);
	sf::Vector2f val = (targetCenter - baseCenter);
	val.x /= base.width + target.width;
	val.y /= base.height + target.height;

	if (abs(val.x) > abs(val.y))
	{
		return val.y > 0.0f ? sf::Vector2f(0.0f, 1.0f) : sf::Vector2f(0.0f, -1.0f);
	}
	else
	{
		return val.x > 0.0f ? sf::Vector2f(1.0f, 0.0f) : sf::Vector2f(-1.0f, 0.0f);
	}
}
