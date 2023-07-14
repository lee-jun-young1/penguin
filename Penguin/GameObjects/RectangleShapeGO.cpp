#include "stdafx.h"
#include "RectangleShapeGO.h"


RectangleShapeGO::RectangleShapeGO()
	:ShapeGameObj(ShapeType::Rectangle)
{
}

void RectangleShapeGO::SetSize(sf::Vector2f size)
{
	((sf::RectangleShape*)shape)->setSize(size);
}

sf::Vector2f RectangleShapeGO::GetSize()
{
	return ((sf::RectangleShape*)shape)->getSize();
}