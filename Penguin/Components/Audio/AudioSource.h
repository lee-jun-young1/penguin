#pragma once
#include "Component.h"
#include <AudioGroup.h>
class AudioSource : public Component
{
protected:
	sf::SoundBuffer* soundClip = nullptr;
	sf::Sound soundStream;
	AudioGroup* audioGroup = nullptr;

	float volume = 100.0f;
	
public:
	AudioSource(GameObject& gameObject);

	// Component��(��) ���� ��ӵ�
	virtual void Init() override;
	virtual void Update(float deltaTime) override;
	virtual void Draw(sf::RenderWindow& window) override;
	virtual void Release() override;

	void SetClip(sf::SoundBuffer* soundClip);

	void Play();
	void Stop();

	void SetVolume(float volume);
	void SetLoop(bool isLoop);

	void OnAudioGroupVolumeChange();
};