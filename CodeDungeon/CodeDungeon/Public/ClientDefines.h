#pragma once
#pragma warning (disable : 4244)
#pragma warning (disable : 4018)
#define _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING 
#define _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS

#include "EngineDefine.h"
#include "ProtocolBuffer.h"

#ifdef _DEBUG
#pragma comment(lib, "EngineD.lib")
#pragma comment(lib, "ProtocolBufferD")
#else
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "ProtocolBuffer")
#endif


namespace Client
{
	namespace DXVALUE
	{
		static const unsigned short g_iWindowWidth = 1280;
		static const unsigned short g_iWindowHeight = 1080;

		static const float g_iWindowHalfWidth = (float)g_iWindowWidth * 0.5f;
		static const float g_iWindowHalfHeight = (float)g_iWindowHeight * 0.5f;
	}

	enum SCENE {
		SCENE_LOGO, SCENE_STAGE1, SCENE_STAGE2, SCENE_END
	};

#define WINDOW_WIDTH					Client::DXVALUE::g_iWindowWidth
#define WINDOW_HEIGHT					Client::DXVALUE::g_iWindowHeight

#define WINDOW_HALF_WIDTH		Client::DXVALUE::g_iWindowHalfWidth
#define WINDOW_HALF_HEIGHT		Client::DXVALUE::g_iWindowHalfHeight

#define FIRST_RESOURCE_FOLDER L"..\\..\\Resource"
#define MAX_LOADSTRING 100
}

#include "ClientProto.h"

static const char* IP_ADDRESS{ "127.0.0.1" };

using namespace Client;