#pragma once
#pragma warning (disable : 4005)
#pragma warning(disable: 4251)
#pragma warning(disable: 4275)
#pragma comment(lib, "dinput8")


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <process.h>
#include <algorithm>
#include <typeinfo>
#include <memory> // for smart ptr
#include <bitset>
#include <map>
#include <thread>
#include <mutex>
#include <locale.h>
#include <utility>
#include <queue>


#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>
#include "d3dxGlobal.h"
#include "DirectXTex.h"
#include <DirectXMath.h>

// for sound
// #include <dsound.h>

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

//#include <DirectXPackedVector.h>
//using namespace DirectX::PackedVector;

#include <DirectXCollision.h>
#include "DirectXTK/Effects.h"




using namespace DirectX;

#include <wrl.h> // ComPtr smartpointer for COM
#include <wrl\client.h>
using namespace std;


#include "PxPhysicsAPI.h"
using namespace physx;


#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "imgui_stdlib.h"
//
#include <fmod.h>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <experimental\filesystem>

#undef min
#undef max
#include <xlnt/xlnt.hpp>

#define FILESYSTEM std::experimental::filesystem

// For Memory Leak
#ifndef _TOOL
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
#endif

#define WINCX	1280
#define WINCY	720
#define SHADOWRATIO 1.f

#include "Engine_Macro.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Struct.h"



//#include "Engine.h"
#include "yaml-cpp\yaml.h"

extern std::mutex PhysxMutex;
extern std::mutex GameObjectMutex;
using namespace Engine;

