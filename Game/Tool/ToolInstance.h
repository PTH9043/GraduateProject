#pragma once
#include "ToolDefines.h"
#include "ToolScene.h"
#include "ToolManager.h"

class Tool
{
public:
	void Init(HINSTANCE hInstance, HWND hWnd);
	void InitImgui(HWND hWnd);
	void BuildToolScene();
	void Update();
	void Destroy();
	void Render();
	void ImguiRender();
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

private:
	shared_ptr<ToolManager> m_pToolManager;
	shared_ptr<CCamera> m_pToolCamera;
	shared_ptr<ToolScene> m_pToolScene;
	ComPtr<ID3D12DescriptorHeap> m_pdxgiSRVDescriptorHeapForImgui;
};