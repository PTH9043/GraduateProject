#include "ToolInstance.h"
#include "Engine.h"
#define TOOL_MODE

void Tool::Init(HINSTANCE hInstance, HWND hWnd)
{
	gGameFrameWork->OnCreate(hInstance, hWnd);
    BuildToolScene();
    InitImgui(hWnd);
}

void Tool::InitImgui(HWND hWnd)
{
    //Imgui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    //win32, API 초기화
    //이를 위해 별도의 SRV Descriptor Heap을 생성
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        DX::ThrowIfFailed(DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_pdxgiSRVDescriptorHeapForImgui)));
    }

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX12_Init(DEVICE.Get(), 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, m_pdxgiSRVDescriptorHeapForImgui.Get(),
        m_pdxgiSRVDescriptorHeapForImgui.Get()->GetCPUDescriptorHandleForHeapStart(),
        m_pdxgiSRVDescriptorHeapForImgui.Get()->GetGPUDescriptorHandleForHeapStart());
}

void Tool::BuildToolScene()
{
    CMDLIST->Reset(CMDALLOCATOR.Get(), NULL);

    m_pToolCamera = make_shared<CCamera>();
    m_pToolCamera->SetViewport(0, 0, RTVDSVDESCRIPTORHEAP->m_nWndClientWidth, RTVDSVDESCRIPTORHEAP->m_nWndClientHeight, 0.0f, 1.0f);
    m_pToolCamera->SetScissorRect(0, 0, RTVDSVDESCRIPTORHEAP->m_nWndClientWidth, RTVDSVDESCRIPTORHEAP->m_nWndClientHeight);
    m_pToolCamera->GenerateProjectionMatrix(1.0f, 500.0f, float(RTVDSVDESCRIPTORHEAP->m_nWndClientWidth) / float(RTVDSVDESCRIPTORHEAP->m_nWndClientHeight), 90.0f);
    m_pToolCamera->CreateShaderVariables(DEVICE.Get(), CMDLIST.Get());
    m_pToolCamera->GenerateViewMatrix(XMFLOAT3(0.0f, 15.0f, -25.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));

    m_pToolScene = make_shared<ToolScene>();
    m_pToolScene->BuildObjects(DEVICE.Get(), CMDLIST.Get(), GRAPHICS_ROOT_SIGNATURE);

    m_pToolManager = make_shared<ToolManager>();
    m_pToolManager->InitCamera(m_pToolCamera);
    m_pToolManager->InitScene(m_pToolScene);

    Util::ExecuteCommandList(CMDLIST, CMDQUEUE, FENCE, ++CMD->m_nFenceValues[RTVDSVDESCRIPTORHEAP->GetSwapChainIndex()], CMD->m_hFenceEvent);
}

void Tool::Update()
{
    gGameFrameWork->RenderBegin();
    gGameFrameWork->Render();
    Render();
    ImguiRender();
    gGameFrameWork->RenderEnd();
}

void Tool::Destroy()
{
	gGameFrameWork->OnDestroy();
    if (m_pToolScene)m_pToolScene->ReleaseObjects();
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Tool::Render()
{
    if (m_pToolScene)
        m_pToolScene->Render(CMDLIST.Get(), m_pToolCamera.get());
}

void Tool::ImguiRender()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //테스트를 위한 예제 윈도우
    //ImGui::ShowDemoWindow();
    if(m_pToolManager) m_pToolManager->DisplayWindow();
    ImGui::Render();

    CMDLIST->SetDescriptorHeaps(1, m_pdxgiSRVDescriptorHeapForImgui.GetAddressOf());
   /* D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = RTVDESCRIPTORHEAP->GetCPUDescriptorHandleForHeapStart();
    d3dRtvCPUDescriptorHandle.ptr += (SWAPCHAIN->GetCurrentBackBufferIndex() * gGameFrameWork->GetSwapChainAndRtvDsvHeap()->GetRTVIncrementSize());
    CMDLIST->OMSetRenderTargets(1u, &d3dRtvCPUDescriptorHandle, FALSE, nullptr);*/
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMDLIST.Get());
}

LRESULT Tool::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

void Tool::OnProcessingMouseMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void Tool::OnProcessingKeyboardMessage(HWND hwnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}
