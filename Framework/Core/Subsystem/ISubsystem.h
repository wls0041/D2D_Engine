#pragma once
#include "Framework.h"

class ISubsystem
{
public:
	ISubsystem(class Context* context)
		: context(context)
	{}
	virtual ~ISubsystem() {}

	virtual void Initialize() = 0;

protected:
	class Context* context;
};