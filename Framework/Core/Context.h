#pragma once
#include "Framework.h"
#include "./Subsystem/ISubsystem.h"

class Context final
{
public:
	Context() {}
	~Context()
	{
		for (int i = static_cast<int>(subsystems.size()) - 1; i >= 0; i--)
			SAFE_DELETE(subsystems[i]);
	}

	void RegisterSubsystem(class ISubsystem* subsystem)
	{
		subsystem->Initialize();
		subsystems.push_back(subsystem);
	}

	template <typename T>
	T* GetSubsystem();

private:
	std::vector<class ISubsystem*> subsystems;
};

template<typename T>
inline T * Context::GetSubsystem()
{
	for (auto subsystem : subsystems)
	{
		if (typeid(T) == typeid(*subsystem))
			return static_cast<T*>(subsystem);
	}
	return nullptr;
}
