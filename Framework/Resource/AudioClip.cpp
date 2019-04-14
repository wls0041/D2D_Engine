#include "Framework.h"
#include "AudioClip.h"

AudioClip::AudioClip(Context * context)
	: IResource(context)
	, sound(nullptr)
	, channel(nullptr)
	, playType(PlayType::Sound)
	, minDistance(1.0f)
	, maxDistance(1000.0f)
	, loopMode(FMOD_LOOP_OFF)
	, rolloffMode(FMOD_3D_LINEARROLLOFF)
	, check(FMOD_OK)
{
	system = context->GetSubsystem<Audio>()->GetFMODSystem();
}

AudioClip::AudioClip(const AudioClip & rhs)
	: IResource(rhs.context)
	, sound(nullptr)
	, channel(nullptr)
	, playType(rhs.playType)
	, minDistance(rhs.minDistance)
	, maxDistance(rhs.maxDistance)
	, loopMode(rhs.loopMode)
	, rolloffMode(rhs.rolloffMode)
	, check(FMOD_OK)
{
	system = context->GetSubsystem<Audio>()->GetFMODSystem();
	LoadFromFile("../../_Assets/Audio/" + rhs.resourcePath);
}

AudioClip::~AudioClip()
{
	if (!sound)
		return;

	check = sound->release();
	assert(check == FMOD_OK);
}

void AudioClip::SaveToFile(const std::string & filePath)
{
}

void AudioClip::LoadFromFile(const std::string & filePath)
{
	switch (playType)
	{
	case PlayType::Sound: CreateSound(filePath); break;
	case PlayType::Stream: CreateStream(filePath); break;
	}
}

void AudioClip::Play()
{
	if (IsChannelValid())
	{
		bool bPaused = false;
		check = channel->getPaused(&bPaused);
		assert(check == FMOD_OK);

		if (bPaused)
		{
			check = channel->setPaused(false);
			assert(check == FMOD_OK);
		}

		if (IsPlaying())
			return;
	}

	check = system->playSound(sound, nullptr, false, &channel);
	assert(check == FMOD_OK);
}

void AudioClip::Pause()
{
	if (!IsChannelValid())
		return;

	//���尡 �Ͻ����� �������� üũ
	bool bPaused = false;
	check = channel->getPaused(&bPaused);
	assert(check == FMOD_OK);

	if (bPaused)
		return;

	//���� �Ͻ�����
	check = channel->setPaused(true);
	assert(check == FMOD_OK);
}

void AudioClip::Stop()
{
	if (!IsChannelValid())
		return;

	//���尡 �÷��������� üũ
	if (!IsPlaying())
		return;

	//���� ����
	check = channel->stop();
	assert(check == FMOD_OK);

	channel = nullptr;
}

void AudioClip::SetLoop(const bool & bLoop)
{
	loopMode = bLoop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	if (!sound)
		return;

	if (bLoop)
	{
		check = sound->setLoopCount(-1);
		assert(check == FMOD_OK);
	}

	check = sound->setMode(GetSoundMode());
	assert(check == FMOD_OK);
}

void AudioClip::SetMute(const bool & bMute)
{
	if (!IsChannelValid())
		return;

	check = channel->setMute(bMute);
	assert(check == FMOD_OK);
}

void AudioClip::SetPriority(const int & priority)
{
	if (!IsChannelValid())
		return;

	check = channel->setPriority(priority);
	assert(check == FMOD_OK);
}

void AudioClip::SetVolume(const float & volume)
{
	if (!IsChannelValid())
		return;

	check = channel->setVolume(volume);
	assert(check == FMOD_OK);
}

void AudioClip::SetPitch(const float & pitch)
{
	if (!IsChannelValid())
		return;

	check = channel->setPitch(pitch);
	assert(check == FMOD_OK);
}

void AudioClip::SetPan(const float & pan)
{
	if (!IsChannelValid())
		return;

	check = channel->setPan(pan);
	assert(check == FMOD_OK);
}

const bool AudioClip::IsPlaying()
{
	if (!IsChannelValid())
		return false;

	bool bPlaying = true;
	check = channel->isPlaying(&bPlaying);
	assert(check == FMOD_OK);

	return bPlaying;
}

void AudioClip::CreateSound(const std::string & filePath)
{
	check = system->createSound
	(
		filePath.c_str(),
		GetSoundMode(),
		nullptr,
		&sound
	);
	assert(check == FMOD_OK);

	check = sound->set3DMinMaxDistance(minDistance, maxDistance);
	assert(check == FMOD_OK);
}

void AudioClip::CreateStream(const std::string & filePath)
{
	check = system->createStream
	(
		filePath.c_str(),
		GetSoundMode(),
		nullptr,
		&sound
	);
	assert(check == FMOD_OK);

	check = sound->set3DMinMaxDistance(minDistance, maxDistance);
	assert(check == FMOD_OK);
}

const int AudioClip::GetSoundMode()
{
	return FMOD_3D | loopMode | rolloffMode;
}

const bool AudioClip::IsChannelValid()
{
	if (!channel)
		return false;

	return true;
}
