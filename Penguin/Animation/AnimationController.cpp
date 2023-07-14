#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceManager.h"

void AnimationController::AddClip(const AnimationClip& newClip)
{
	if (clips.find(newClip.id) == clips.end())
	{
		clips.insert({ newClip.id, newClip });
	}
}

void AnimationController::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * speed;

	if (accumTime < clipFrameTime)
		return;

	accumTime = 0.0f;
	++currentFrame;

	if (currentFrame == totalFrame)
	{
		if (!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();
			Play(id, false);
			return;
		}

		else
		{
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				return;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			}
		}
	}
	if (currentClip->frames[currentFrame].action != nullptr)
	{
		currentClip->frames[currentFrame].action();
	}
	if (target != nullptr)
	{
		SetFrame(currentClip->frames[currentFrame]);
	}
}

void AnimationController::Play(const std::string& clipID, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop();
		}
	}

	auto findClip = clips.find(clipID);
	if (findClip == clips.end())
	{
		Stop();
		std::cout << "ERR::NOT EXIST CLIP" << std::endl;
		return;
	}

	isPlaying = true;
	currentClip = &findClip->second;
	currentFrame = 0;
	totalFrame = currentClip->frames.size();
	clipFrameTime = 1.0f / currentClip->fps;
	accumTime = 0.0f;

	SetFrame(currentClip->frames[currentFrame]);
}

void AnimationController::PlayQueue(const std::string& clipID)
{
	queue.push(clipID);
}

float AnimationController::GetClipDuration()
{
	return clipFrameTime * totalFrame;
}

void AnimationController::Stop()
{
	isPlaying = false;
}

std::string AnimationController::GetCurrentClipName()
{
	return currentClip->id;
}

void AnimationController::SetFrame(const AnimationFrame& frame)
{
	sf::Texture* tex = RESOURCE_MANAGER.GetTexture(frame.textureID);
	target->setTexture(*tex);
	target->setTextureRect(frame.tecCoord);
}
