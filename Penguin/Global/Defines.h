#pragma once


enum class Languages
{
	KOR,
	ENG,
	JPN,
	COUNT,
};


enum class SceneId
{
	None = -1,
	Game,
	Dev1,
	Dev2,
	Count,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,
};

enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM,
};

enum class ComponentType
{
	RigidBody,
	Collider,
	Animation,
	Audio,
};

enum class ColliderType
{
	Box,
	Circle,
};

enum class PhysicsLayer
{
	Background = 1,
	Wall,
	Count
};

enum class ShapeType
{
	None,
	Circle,
	Convex,
	Rectangle,
};

#define UILayer 100
#include "AnimationClip.h"