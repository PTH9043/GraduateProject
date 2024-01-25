#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H

#pragma warning (disable : 4251)

#pragma region DX12

#pragma comment (lib, "d3d12")
#pragma comment (lib, "dxgi")
#pragma comment (lib, "dxguid")
#pragma comment (lib, "d3dcompiler")
#pragma comment (lib, "dinput8")


#ifdef _DEBUG
#pragma comment (lib, "DirectXTKd12")
#pragma comment (lib, "fmodL_vc")
#pragma comment (lib, "PTHLib_Debug")
#else
#pragma comment (lib, "DirectXTK12")
#pragma comment (lib, "fmod_vc")
#pragma comment (lib, "PTHLib")
#endif

#pragma comment (lib, "libxl")

// Use Atomic Shared PTr
#define _USE_ATOMIC
// Use Debugging Values
//#define _USE_DEBUGGING

#include <d3d12.h>
#include <dxgi1_4.h>
#include <dxgi.h>
#include <DXGIDebug.h>

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <DirectXTK/GraphicsMemory.h>
#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/DescriptorHeap.h>
#include "DirectXTK/d3dx12.h"


#include "MathLib/CusMath.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "DirectXTK/GamePad.h"

#include <Windows.h>
#include <iostream>
#include <array>
#include <concepts>
#include <string_view>
#include <mutex>
#include <thread>
#include <set>
#include <unordered_set>
#include <queue>
#include <list>
#include <vector>
#include <set>
#include <random>
#include <filesystem>
#include <unordered_map>
#include <atomic>

#include "EngineConcept.h"
#include "UDelegate.h"

#include "EngineTypedef.h"
#include "EngineEnum.h"
#include "EngineFunction.h"
#include "EngineMacro.h"
#include "EngineMake.h"
#include "EngineStruct.h"
#include "EngineShaderParam.h"


#pragma endregion DX12

namespace Engine {
	namespace DXVALUE {
		static const _float3					 g_BasicScale = _float3(1.f, 1.f, 1.f);
		static constexpr _uint				 g_maxRegisterCnt{ REGISTER_CNT };
		static const _wstring				 g_EmptyWstr{ L"" };
		static const _wstring				 g_OutFolderExtension{ L".bin" };
	}
}

#define BASIC_SCALE										Engine::DXVALUE::g_BasicScale
#define MAX_REGISTER_CNT						Engine::DXVALUE::g_maxRegisterCnt
#define EMPTY_WSTR									Engine::DXVALUE::g_EmptyWstr
#define DEFAULT_OUTFOLDEREXTENSION		Engine::DXVALUE::g_OutFolderExtension

using namespace Engine;

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UENGINEDEFINES_H