#pragma once

class ScriptInstance final
{
	ScriptInstance();
	~ScriptInstance();

	ScriptInstance(const ScriptInstance&) = delete;
	ScriptInstance &operator=(const ScriptInstance&) = delete;
};