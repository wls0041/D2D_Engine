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

	const bool InitializeSubsystems() {
		for (auto subsystem : subsystems) {
			if (!subsystem->Initialize()) {
				Log::WriteFormatError("Failed to Initialize %s\n", typeid(*subsystem).name());
				return false;
			}
		}
		return true;
	}

	template <typename T>
	T* RegisterSubsystem();

	template <typename T>
	T* GetSubsystem();

private:
	std::vector<class ISubsystem*> subsystems;
};

template<typename T>
inline T * Context::RegisterSubsystem()
{
	//static_assert : compileŸ�ӿ� ������ ����. ��: ��Ÿ�� ���� �� �����ϸ����� ��������, ��: �����Ͻð��� �����
	//is base of : �θ� Isubsystem���� Ȯ��
	static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem"); 
	return static_cast<T*>subsystems.emplace_back(new T(this)); //emplace_back�� ��ȯ���� ������ Ÿ��(ISubsystem)->����ȯ
}

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
