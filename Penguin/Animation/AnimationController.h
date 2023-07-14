#pragma once
class AnimationController
{
protected:
	bool isPlaying = false;

	std::unordered_map<std::string, AnimationClip> clips;
	std::queue<std::string> queue;

	float speed = 1.0f;

	AnimationClip* currentClip = nullptr;
	int currentFrame = -1;
	int totalFrame = 0;
	float clipFrameTime = 0.0f;
	float accumTime = 0.0f;

	sf::Sprite* target = nullptr;
public:
	void AddClip(const AnimationClip& newClip);
	void SetTarget(sf::Sprite* sprite) { target = sprite; }
	sf::Sprite* GetTarget() const { return target; }
	bool IsPlaying() { return isPlaying; }
	void SetSpeed(float s) { speed = s; }
	float GetSpeed() const { return speed; }

	void Update(float dt);

	void Play(const std::string& clipID, bool clearQueue = true);
	void PlayQueue(const std::string& clipID);
	float GetClipDuration();
	void Stop();
	std::string GetCurrentClipName();

	void SetFrame(const AnimationFrame& frame);
};

