#pragma once
#include "IComponent.h"

class AudioSource final : public IComponent
{
public:
	AudioSource(class Context* context, class GameObject *object, class Transform *transform);
	virtual ~AudioSource();

	void OnInitialize() override;
	void OnStart() override; //�����ϴ� ����
	void OnUpdate() override;
	void OnStop() override; //���� ��
	void OnDestroy() override;

	void SetAudioClip(const std::string& filePath);

	void Play();
	void Pause();
	void Stop();

	const bool& IsLoop() const { return bLoop; }
	const bool& IsMute() const { return bMute; }
	const int& GetPriority() const { return priority; }
	const float& GetVolume() const { return volume; }
	const float& GetPitch() const { return pitch; }
	const float& GetPan() const { return pan; }

	void SetIsLoop(const bool& bLoop);
	void SetIsMute(const bool& bMute);
	void SetPriority(const int& priority);
	void SetVolume(const float& volume);
	void SetPitch(const float& pitch);
	void SetPan(const float& pan);

private:
	class Context* context;
	class AudioClip* audioClip;
	std::string filePath;
	bool bLoop;
	bool bMute;
	int priority;
	float volume;
	float pitch;
	float pan;
};