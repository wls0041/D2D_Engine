#pragma once

//#define OPTIMIZATION //dx11_helper에 분기점으로 작용

#include <Windows.h>
#include <assert.h>
#include <time.h>

//STL
#include <string>
#include <vector>
#include <map>
#include <deque>
#include <functional>
#include <chrono>
#include <fstream>
#include <thread>
#include <mutex>

//Direct Write
#include <d2d1_1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//DirectX
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//FMOD
#include <FMOD\fmod.hpp>
#include <FMOD\fmod_errors.h>
#pragma comment(lib, "fmod_vc.lib")

typedef unsigned int uint;

//Macro Function
#define SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE(p)			{ if (p) { delete (p); (p) = nullptr; } } 
#define SAFE_DELETE_ARRAY(p)	{ if (p) { delete[] (p); (p) = nullptr; } } 

//Math
#include "./Math/Math.h"
#include "./Math/Vector2.h"
#include "./Math/Vector3.h"
#include "./Math/Color.h"
#include "./Math/Matrix.h"
#include "./Math/BoundBox.h"

//Utility
#include "./Utility/GeometryUtility.h"
#include "./Utility/FileSystem.h"
#include "./Utility/Xml.h"
#include "./Utility/GUIDGenerator.h"

//Log
#include "./Log/Log.h"

//Mainsystem
#include "./Core/Settings.h"
#include "./Core/Context.h"
#include "./Core/Engine.h"

//EventSystem
#include "./Core/EventSystem/EventSystem.h"

//BasicData
#include "./Core/D3D11/BasicData/Config.h"
#include "./Core/D3D11/BasicData/Vertex.h"
#include "./Core/D3D11/BasicData/Geometry.h"

//Subsystem
#include "./Core/Subsystem/Timer.h"
#include "./Core/Subsystem/Input.h"
#include "./Core/Subsystem/Audio.h"
#include "./Core/Subsystem/Graphics.h"
#include "./Core/Subsystem/ResourceManager.h"
#include "./Core/Subsystem/SceneManager.h"
#include "./Core/Subsystem/ColliderManager.h"
#include "./Core/Subsystem/Renderer.h"

//Wrapper Class
#include "./Core/D3D11/IAStage/VertexBuffer.h"
#include "./Core/D3D11/IAStage/IndexBuffer.h"
#include "./Core/D3D11/IAStage/InputLayout.h"
#include "./Core/D3D11/ShaderStage/VertexShader.h"
#include "./Core/D3D11/ShaderStage/PixelShader.h"
#include "./Core/D3D11/ShaderStage/ConstantBuffer.h"
#include "./Core/D3D11/OMStage/RenderTexture.h"
#include "./Core/D3D11/OMStage/BlendState.h"

//Resource
#include "./Resource/Texture.h"
#include "./Resource/Animation.h"
#include "./Resource/AudioClip.h"
#include "./Resource/Material.h"
#include "./Resource/Mesh.h"
