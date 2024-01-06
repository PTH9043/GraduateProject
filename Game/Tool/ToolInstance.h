#pragma once

#pragma comment(lib, "Engine.lib")
#include "../Engine/EnginePch.h"

//Imgui 사용을 위한 헤더
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx12.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imstb_rectpack.h"
#include "ImGui/imstb_textedit.h"
#include "ImGui/imstb_truetype.h"

class Tool
{
public:
	void Init(HINSTANCE hInstance, HWND hWnd);
	void Update();
	void Destroy();
	void Render();
private:
	ComPtr<ID3D12DescriptorHeap> m_pdxgiSRVDescriptorHeapForImgui;
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};