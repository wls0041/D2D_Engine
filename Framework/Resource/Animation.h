#pragma once
#include "IResource.h"

enum class RepeatType : uint
{
	Once, Loop
};

struct Keyframe
{
	Keyframe() { ZeroMemory(this, sizeof(Keyframe)); }
	Keyframe
	(
		const std::string& filePath,
		const Vector2& offset,
		const Vector2& size,
		const float& time
	)
		: filePath(filePath)
		, offset(offset)
		, size(size)
		, time(time)
	{}

	std::string filePath;
	Vector2 offset;
	Vector2 size;
	float time;
};

class Animation : public IResource
{
public:
	Animation(class Context* context);
	virtual ~Animation();

	void SaveToFile(const std::string& filePath) override;
	void LoadFromFile(const std::string& filePath) override;

	const RepeatType& GetRepeatType() const { return repeatType; }
	const std::vector<Keyframe>& GetKeyframes() const { return keyframes; }
	Keyframe* GetKeyframeFromIndex(const uint& index);
	const uint GetKeyframeCount() const { return keyframes.size(); }

	void SetRepeatType(const RepeatType& repeatType) { this->repeatType = repeatType; }

	void AddKeyframe
	(
		const std::string& filePath,
		const Vector2& offset,
		const Vector2& size,
		const float& time
	);

private:
	RepeatType repeatType;
	std::vector<Keyframe> keyframes;
};