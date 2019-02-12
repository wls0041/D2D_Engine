#include "Framework.h"
#include "EffectManager.h"
#include "../../Resource/Effect.h"

EffectManager::EffectManager(Context * context)
	: ISubsystem(context)
{
}

EffectManager::~EffectManager()
{
	for (auto effect : effects)
		SAFE_DELETE(effect);

	effects.clear();
	effects.shrink_to_fit();
}

void EffectManager::Initialize()
{
}

void EffectManager::AddEffect(const std::string & textureFilePath, const std::string & animationFilePath, const Vector3 & position, const Vector3 & scale)
{
	auto effect = new Effect(context);

	effect->SetTexture(textureFilePath);
	effect->SetAnimation(animationFilePath);
	effect->SetPosition(position);
	effect->SetScale(scale);
	effect->SetIsExist(true);

	effects.emplace_back(effect);
}

void EffectManager::Update()
{
	//for (uint i = 0; i < effects.size();)
	//{
	//	effects[i]->Update();

	//	if (!effects[i]->IsExist())
	//		effects.erase(effects.begin() + i);
	//	else
	//		i++;
	//}

	for (auto iter = effects.begin(); iter != effects.end();)
	{
		auto effect = *iter;
		effect->Update();

		if (!effect->IsExist())
		{
			SAFE_DELETE(effect);
			iter = effects.erase(iter);
		}
		else
			iter++;
	}
}

void EffectManager::Render()
{
	for (auto effect : effects)
		effect->Render();
}
