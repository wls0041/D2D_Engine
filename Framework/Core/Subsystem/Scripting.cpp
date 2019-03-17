#include "Framework.h"
#include "Scripting.h"
#include <AngelScript_2.33.0/angelscript.h>
#include <AngelScript_2.33.0/scriptstdstring/scriptstdstring.h> //addon���
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

const bool Scripting::Initialize() //��ũ��Ʈ ����
{
	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	if (!scriptEngine) return false;

	RegisterStdString(scriptEngine);

	auto scriptInterface = new ScriptInterface();
	scriptInterface->Register(context, scriptEngine);

	scriptEngine->SetMessageCallback(asMETHOD(Scripting, Message_CallBack), this, asCALL_THISCALL); //scripting�� �ִ� message_callback�� this�� �ְڴ�
	scriptEngine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true); //line cue ���� ����

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

//asiscriptcontext�� �Լ��� ȣ���ϴµ� ����
//overhead(���ʿ��� �����ð�)�� ���ϱ� ���� pooling�ؾ� �Ѵٰ� �޴��� ����. �׷��� �̷� ������ ó��.(������ ����� ������ �ִ°ɷ� ��)
auto Scripting::RequestContext() -> class asIScriptContext *
{
	asIScriptContext *ctx = nullptr;
	if (!ctxs.empty()) {
		ctx = *ctxs.rbegin(); //���� ���� ���� != end
		ctxs.pop_back();
	}
	else ctx = scriptEngine->CreateContext();
	
	return ctx;
}

//angelscript�Լ��� ȣ���� �� asiScriptcontext*�� �ٽ� ��ȯ�ؾ���
//pooling ����� ���ؼ�
void Scripting::ReturnContext(asIScriptContext * ctx)
{
	if (!ctx) {
		Log::Error("Scripting::Return Context : \"ctx\" is nullptr");
		return;
	}

	ctxs.push_back(ctx);
	ctx->Unprepare(); //�غ�����
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
