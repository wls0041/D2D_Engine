#pragma once
#include "Framework.h"

class IPipeline
{
public:
	IPipeline() { id = GUIDGenerator::Generate(); }
	virtual ~IPipeline() = default;

	const uint &GetID() const { return id; }

private:
	uint id;
};