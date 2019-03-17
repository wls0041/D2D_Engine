#pragma once
#include "ISubsystem.h"

class Scripting final : public ISubsystem
{
public:
	Scripting(class Context *context);
	~Scripting();

	Scripting(const Scripting&) = delete;
	Scripting &operator=(const Scripting&) = delete;

	const bool Initialize() override;
	void Clear();

	auto GetAsIScriptEngine() -> class asIScriptEngine*;
	auto RequestContext() -> class asIScriptContext*;
	
	void ReturnContext(class asIScriptContext *ctx);
	auto ExecuteCall(class asIScriptFunction *scriptFunc, class asIScriptObject *obj) -> const bool; //stl functional과 비슷 obj와 함수를 받아 실행
	
	void DiscardModule(const std::string &moduleName);

private:
	void LogExceptionInfo(class asIScriptContext *ctx);
	void Message_CallBack(const struct asSMessageInfo &msg);

private:
	class asIScriptEngine *scriptEngine;
	std::vector<class asIScriptContext*> ctxs;
};