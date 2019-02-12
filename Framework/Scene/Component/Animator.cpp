#include "Framework.h"
#include "Animator.h"

Animator::Animator(Context * context)
	: context(context)
	, mode(AnimationMode::Play)
	, curFrameNumber(0)
	, frameTimer(0.0f)
	, curAnimation(nullptr)
{
	timer = context->GetSubsystem<Timer>();
	resourceMgr = context->GetSubsystem<ResourceManager>();
}

Animator::~Animator()
{
	animations.clear();
}

void Animator::SaveToFile(const std::string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	doc.LinkEndChild(decl);

	Xml::XMLElement* root = doc.NewElement("Animator");
	doc.LinkEndChild(root);

	for (auto& animation : animations)
	{
		Xml::XMLElement* firstElem = doc.NewElement("Animation");
		root->LinkEndChild(firstElem);

		firstElem->SetAttribute("Name", animation.second->GetResourceName().c_str());
		firstElem->SetAttribute("Type", static_cast<int>(animation.second->GetRepeatType()));

		for (auto& keyframe : animation.second->GetKeyframes())
		{
			Xml::XMLElement* secondElem = doc.NewElement("Keyframe");
			firstElem->LinkEndChild(secondElem);

			secondElem->SetAttribute("FilePath", keyframe.filePath.c_str());
			secondElem->SetAttribute("OffsetX", keyframe.offset.x);
			secondElem->SetAttribute("OffsetY", keyframe.offset.y);
			secondElem->SetAttribute("SizeX", keyframe.size.x);
			secondElem->SetAttribute("SizeY", keyframe.size.y);
			secondElem->SetAttribute("Time", keyframe.time);
		}
	}
	doc.SaveFile(filePath.c_str());
}

void Animator::LoadFromFile(const std::string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(filePath.c_str());
	assert(error == Xml::XMLError::XML_SUCCESS);

	Xml::XMLElement* root = doc.FirstChildElement();
	Xml::XMLElement* firstElem = root->FirstChildElement();
	for (; firstElem != nullptr; firstElem = firstElem->NextSiblingElement())
	{
		std::string name = firstElem->Attribute("Name");
		RepeatType type = static_cast<RepeatType>(firstElem->IntAttribute("Type"));

		Animation* animation = new Animation(context);
		animation->SetResourceName(name);
		animation->SetRepeatType(type);

		Xml::XMLElement* secondElem = firstElem->FirstChildElement();
		for (; secondElem != nullptr; secondElem = secondElem->NextSiblingElement())
		{
			std::string filePath = secondElem->Attribute("FilePath");
			float offsetX = secondElem->FloatAttribute("OffsetX");
			float offsetY = secondElem->FloatAttribute("OffsetY");
			float sizeX = secondElem->FloatAttribute("SizeX");
			float sizeY = secondElem->FloatAttribute("SizeY");
			float time = secondElem->FloatAttribute("Time");

			animation->AddKeyframe
			(
				filePath,
				{ offsetX, offsetY },
				{ sizeX, sizeY },
				time
			);
		}
		
		RegisterAnimation(animation);
	}
}

void Animator::Update()
{
	bool bCheck = true;
	bCheck &= curAnimation != nullptr;
	bCheck &= IsPlay();

	if (bCheck)
	{
		frameTimer += timer->GetDeltaTimeMs();

		if (frameTimer > curAnimation->GetKeyframeFromIndex(curFrameNumber)->time)
		{
			curFrameNumber++;

			switch (curAnimation->GetRepeatType())
			{
			case RepeatType::Once:
				if (curFrameNumber >= curAnimation->GetKeyframeCount())
				{
					curFrameNumber = curAnimation->GetKeyframeCount() - 1;
					Pause();
				}
				break;
			case RepeatType::Loop:
				curFrameNumber %= curAnimation->GetKeyframeCount();
				break;
			}

			frameTimer = 0.0f;
		}
	}
}

Keyframe * Animator::GetCurrentkeyframe()
{
	if (!curAnimation)
		return nullptr;

	return curAnimation->GetKeyframeFromIndex(curFrameNumber);
}

void Animator::SetCurrentAnimation(const std::string & animationName)
{
	assert(animations.count(animationName) > 0);
	curFrameNumber = 0;
	frameTimer = 0.0f;
	curAnimation = animations[animationName];
}

void Animator::RegisterAnimation(const std::string & filePath)
{
	auto animation = resourceMgr->Load<Animation>(filePath);

	assert(animation);
	assert(animations.count(animation->GetResourceName()) < 1);
	animations[animation->GetResourceName()] = animation;
}

void Animator::RegisterAnimation(Animation * animation)
{
	assert(animation);
	assert(animations.count(animation->GetResourceName()) < 1);
	animations[animation->GetResourceName()] = animation;

	resourceMgr->RegisterResource(animation);
}

void Animator::Play()
{
	mode = AnimationMode::Play;
	frameTimer = 0.0f;
}

void Animator::Pause()
{
	mode = AnimationMode::Pause;
}

void Animator::Stop()
{
	mode = AnimationMode::Stop;
	frameTimer = 0.0f;
	curFrameNumber = 0;
}
