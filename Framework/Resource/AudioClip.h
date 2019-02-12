#pragma once
#include "IResource.h"

enum class PlayType : uint
{
	Sound,
	Stream
};

class AudioClip : public IResource
{
public:
	AudioClip(class Context* context);
	AudioClip(const AudioClip& rhs); //복사생성자
	virtual ~AudioClip();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	void Play();
	void Pause();
	void Stop();

	void SetLoop(const bool& bLoop);
	void SetMute(const bool& bMute);
	void SetPriority(const int& priority);
	void SetVolume(const float& volume);
	void SetPitch(const float& pitch);
	void SetPan(const float& pan);

	void SetPlayType(const PlayType& type) { this->playType = type; }

	const bool IsPlaying();

private:
	void CreateSound(const std::string& filePath);
	void CreateStream(const std::string& filePath);

	const int GetSoundMode();
	const bool IsChannelValid();

private:
	FMOD::System* system;
	FMOD::Sound* sound;
	FMOD::Channel* channel;

	PlayType playType;
	int loopMode;
	int rolloffMode;
	float minDistance;
	float maxDistance;
	int check;
};