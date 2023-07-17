#pragma once
#include "Component.h"
#include "Animation.h"


struct Transition;

struct AnimationState
{
	AnimationClip clip;
	std::unordered_map<std::string, Transition> transitions;
};

struct Transition
{
	bool notAllowQuitTime;
	int quitTime;
	AnimationState* enterAnimationState;
	int enterTime;
};

//FSM
class Animator : public Animation
{
protected:
	std::unordered_map<std::string, AnimationState> states;
	AnimationState* currentState;

	string currentEvent;
public:
	Animator(SpriteGO& spriteGO);
	virtual ~Animator() override;
	void AddTransition(const std::string& stateName, const std::string& transitionName, Transition transition);
	void AddState(const std::string& stateName, const AnimationClip& newClip);

	AnimationState* GetState(const std::string& stateName);

	void SetEvent(std::string eventID);
	void SetState(std::string stateID);

	virtual void Update(float dt) override;

	void LoadFromFile(std::string path);
};

