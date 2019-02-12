#pragma once

enum class AnimationMode : uint
{
	Play,
	Pause,
	Stop
};

class Animator
{
public:
	Animator(class Context* context);
	virtual  ~Animator();

	void SaveToFile(const std::string& filePath);
	void LoadFromFile(const std::string& filePath);

	void Update();

	class Animation* GetCurrentAnimation() const { return curAnimation; }
	struct Keyframe* GetCurrentkeyframe();

	void SetCurrentAnimation(const std::string& animationName);

	void RegisterAnimation(const std::string& filePath);
	void RegisterAnimation(class Animation* animation);

	void Play();
	void Pause();
	void Stop();

	const bool IsPlay() const { return mode == AnimationMode::Play ? true : false; }
	const bool IsPause() const { return mode == AnimationMode::Pause ? true : false; }
	const bool IsStop() const { return mode == AnimationMode::Stop ? true : false; }

private:
	class Context* context;
	class Timer* timer;
	class ResourceManager* resourceMgr;

	AnimationMode mode;
	uint curFrameNumber;
	float frameTimer;

	class Animation* curAnimation;
	std::map<std::string, class Animation*> animations;
};