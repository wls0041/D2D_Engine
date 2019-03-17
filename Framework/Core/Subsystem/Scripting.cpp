#include "Framework.h"
#include "Scripting.h"
#include <AngelScript_2.33.0/angelscript.h>
#include <AngelScript_2.33.0/scriptstdstring/scriptstdstring.h> //addon기능
#include "../../Script/ScriptInterface.h"

Scripting::Scripting(Context * context) : ISubsystem(context), scriptEngine(nullptr)
{
}

Scripting::~Scripting()
{
	Clear();

	if (scriptEngine) {
		scriptEngine->ShutDownAndRelease();
		scriptEngine = nullptr;
	}
}

const bool Scripting::Initialize() //스크립트 연동
{
	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if (!scriptEngine) return false;

	RegisterStdString(scriptEngine);

	auto scriptInterface = new ScriptInterface();
	scriptInterface->Register(context, scriptEngine);

	scriptEngine->SetMessageCallback(asMETHOD(Scripting, Message_CallBack), this, asCALL_THISCALL); //scripting에 있는 message_callback을 this에 넣겠다
	scriptEngine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true); //line cue 없이 빌드

	SAFE_DELETE(scriptInterface);
	return true;
}

void Scripting::Clear()
{
	for (auto &ctx : ctxs) ctx->Release();
	ctxs.clear();
	ctxs.shrink_to_fit();
}

auto Scripting::GetAsIScriptEngine() -> class asIScriptEngine *
{
	return scriptEngine;
}

//asiscriptcontext는 함수를 호출하는데 사용됨
//overhead(불필요한 지연시간)를 피하기 위해 pooling해야 한다고 메뉴얼에 나옴. 그래서 이런 식으로 처리.(없으면 만들고 있으면 있는걸로 씀)
auto Scripting::RequestContext() -> class asIScriptContext *
{
	asIScriptContext *ctx = nullptr;
	if (!ctxs.empty()) {
		ctx = *ctxs.rbegin(); //뒤쪽 시작 원소 != end
		ctxs.pop_back();
	}
	else ctx = scriptEngine->CreateContext();
	
	return ctx;
}

//angelscript함수를 호출한 후 asiScriptcontext*는 다시 반환해야함
//pooling 사용을 위해서
void Scripting::ReturnContext(asIScriptContext * ctx)
{
	if (!ctx) {
		Log::Error("Scripting::Return Context : \"ctx\" is nullptr");
		return;
	}

	ctxs.push_back(ctx);
	ctx->Unprepare(); //준비해제
}

auto Scripting::ExecuteCall(asIScriptFunction * scriptFunc, asIScriptObject * obj) -> const bool
{
	auto ctx = RequestContext();
	ctx->Prepare(scriptFunc);
	ctx->SetObject(obj);

	auto result = ctx->Execute();
	if (result == asEXECUTION_EXCEPTION) {
		LogExceptionInfo(ctx);
		ReturnContext(ctx);
		return false;
	}

	ReturnContext(ctx);
	return true;
}

void Scripting::DiscardModule(const std::string & moduleName)
{
	scriptEngine->DiscardModule(moduleName.c_str());
}

void Scripting::LogExceptionInfo(asIScriptContext * ctx)
{
	auto exceptionDesc = ctx->GetExceptionString();
	auto function = ctx->GetExceptionFunction();
	auto functionDecl = function->GetDeclaration();
	auto moduleName = function->GetModuleName();
	auto scriptPath = function->GetScriptSectionName();
	auto scriptFile = FileSystem::GetFileNameFromFilePath(scriptPath);
	auto exceptionLine = std::to_string(ctx->GetExceptionLineNumber());

	Log::WriteFormatError("%s, at line %s, in function %s, in script %s", exceptionDesc, exceptionLine, functionDecl, scriptFile);
}

void Scripting::Message_CallBack(const asSMessageInfo & msg)
{
	auto fileName = FileSystem::GetFileNameFromFilePath(msg.section);
	auto message = msg.message;

	std::string finalMessage;
	if (fileName != "") finalMessage = fileName + " " + message;
	else finalMessage = message;
}
