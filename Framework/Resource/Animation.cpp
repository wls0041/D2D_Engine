#include "Framework.h"
#include "Animation.h"

Animation::Animation(Context * context)
	: IResource(context)
	, repeatType(RepeatType::Loop)
{
}

Animation::~Animation()
{
	keyframes.clear();
	keyframes.shrink_to_fit();
}

void Animation::SaveToFile(const std::string & filePath)
{
	Xml::XMLDocument doc;

	Xml::XMLDeclaration* decl = doc.NewDeclaration();
	doc.LinkEndChild(decl);

	Xml::XMLElement* root = doc.NewElement("Animation");
	doc.LinkEndChild(root);

	root->SetAttribute("Name", resourceName.c_str());
	root->SetAttribute("Type", static_cast<int>(repeatType));

	for (auto& keyframe : keyframes)
	{
		Xml::XMLElement* firstElem = doc.NewElement("Keyframe");
		root->LinkEndChild(firstElem);

		firstElem->SetAttribute("FilePath", keyframe.filePath.c_str());
		firstElem->SetAttribute("OffsetX", keyframe.offset.x);
		firstElem->SetAttribute("OffsetY", keyframe.offset.y);
		firstElem->SetAttribute("SizeX", keyframe.size.x);
		firstElem->SetAttribute("SizeY", keyframe.size.y);
		firstElem->SetAttribute("Time", keyframe.time);
	}

	doc.SaveFile(filePath.c_str());
}

void Animation::LoadFromFile(const std::string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(filePath.c_str());
	assert(error == Xml::XMLError::XML_SUCCESS);

	Xml::XMLElement* root = doc.FirstChildElement();
	resourceName = root->Attribute("Name");
	repeatType = static_cast<RepeatType>(root->IntAttribute("Type"));

	Xml::XMLElement* firstElem = root->FirstChildElement();
	for (; firstElem != nullptr; firstElem = firstElem->NextSiblingElement())
	{
		std::string filePath = firstElem->Attribute("FilePath");
		float offsetX = firstElem->FloatAttribute("OffsetX");
		float offsetY = firstElem->FloatAttribute("OffsetY");
		float sizeX = firstElem->FloatAttribute("SizeX");
		float sizeY = firstElem->FloatAttribute("SizeY");
		float time = firstElem->FloatAttribute("Time");

		AddKeyframe(filePath, { offsetX, offsetY }, { sizeX, sizeY }, time);
	}
}

Keyframe * Animation::GetKeyframeFromIndex(const uint & index)
{
	if (keyframes.size() <= index)
		return nullptr;

	return &keyframes[index];
}

void Animation::AddKeyframe(const std::string & filePath, const Vector2 & offset, const Vector2 & size, const float & time)
{
	keyframes.emplace_back(filePath, offset, size, time);
}
