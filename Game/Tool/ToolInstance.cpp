#include "ToolInstance.h"
#include "Engine.h"

void Tool::Init(HINSTANCE hInstance, HWND hWnd)
{
	gGameFrameWork->OnCreate(hInstance, hWnd);

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

void Tool::Update()
{
    gGameFrameWork->RenderBegin();
    gGameFrameWork->Render();
    Render();
    gGameFrameWork->RenderEnd();
}

void Tool::Destroy()
{
	gGameFrameWork->OnDestroy();
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Tool::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //테스트를 위한 예제 윈도우
    ImGui::ShowDemoWindow();

    ImGui::Render();

    CMDLIST->SetDescriptorHeaps(1, m_pdxgiSRVDescriptorHeapForImgui.GetAddressOf());
   /* D3D12_CPU_DESCRIPTOR_HANDLE d3dRtvCPUDescriptorHandle = RTVDESCRIPTORHEAP->GetCPUDescriptorHandleForHeapStart();
    d3dRtvCPUDescriptorHandle.ptr += (SWAPCHAIN->GetCurrentBackBufferIndex() * gGameFrameWork->GetSwapChainAndRtvDsvHeap()->GetRTVIncrementSize());
    CMDLIST->OMSetRenderTargets(1u, &d3dRtvCPUDescriptorHandle, FALSE, nullptr);*/
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMDLIST.Get());
}
