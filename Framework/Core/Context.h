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
	//static_assert : compile타임에 에러가 잡힘. 장: 런타임 들어가기 전 컴파일만으로 에러잡음, 단: 컴파일시간이 길어짐
	//is base of : 부모가 Isubsystem인지 확인
	static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem"); 
	return static_cast<T*>subsystems.emplace_back(new T(this)); //emplace_back의 반환형은 내부의 타입(ISubsystem)->형변환
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
