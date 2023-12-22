#include "ToolDefines.h"
#include "TMainView.h"
#include "UGameInstance.h"
#include "UTexture.h"

TMainView::TMainView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "MainView"), 
		m_stMainDesc{},
	m_stDebuggingView{},
    m_vecOpenImGuies{},
    m_dShowDeltaTime{0.0},
    m_isDockBuilding{false}
{
}

void TMainView::Free()
{
 
}

HRESULT TMainView::NativeConstruct()
{
    m_stMainDesc = MAINDESC(
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus
        | ImGuiWindowFlags_NoBackground
        , ImGuiDockNodeFlags_PassthruCentralNode);
    return S_OK;
}

void TMainView::InsertImGuiView(const CSHPTRREF<TImGuiView> _spImGuiView)
{
    RETURN_CHECK(nullptr == _spImGuiView, ;);
    m_vecImGuies.push_back(_spImGuiView);
}

HRESULT TMainView::LoadResource()
{
	return S_OK;
}

HRESULT TMainView::ReleaseResource()
{
	return S_OK;
}

void TMainView::TickActive(const _double& _dTimeDelta)
{
    m_dShowDeltaTime = _dTimeDelta;
    for (auto& iter : m_vecOpenImGuies)
    {
        iter->Tick(_dTimeDelta);
    }
}

void TMainView::LateTickActive(const _double& _dTimeDetla)
{
    for (auto& iter : m_vecOpenImGuies)
    {
        iter->LateTick(_dTimeDetla);
    }
}

void TMainView::RenderActive()
{
    ImGuiViewport* pViewPort = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(pViewPort->WorkPos, ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(pViewPort->WorkSize, ImGuiCond_Appearing);
    ImGui::Begin(GetName().c_str(), nullptr, m_stMainDesc.imgWindowFlags);
    {
        // Basic info
        ImGuiContext& g = *GImGui;
        ImGuiIO& io = g.IO;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::Text("Application Delta %1f", m_dShowDeltaTime);

        m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
        ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{0, 0}, m_stMainDesc.imgDockNodeFlags);

        RenderMenu();

        for (auto& iter : m_vecOpenImGuies)
        {
            iter->Render();
        }
    }
    ImGui::End();
}

void TMainView::RenderMenu()
{
    ImGui::BeginMainMenuBar();
    {
        if(ImGui::BeginMenu("Tools"))
        {
            for (auto& iter : m_vecImGuies)
            {
                _bool isOpen = iter->IsOpen();
                if (ImGui::MenuItem(iter->GetName().c_str(), "", &isOpen))
                {
                    if (false == iter->IsActive()) {
                        m_vecOpenImGuies.push_back(iter);
                    }
                    iter->OpenImGui();
                }
            }
            ImGui::EndMenu();
        }

        for (VECTOR<SHPTR<TImGuiView>>::iterator iter = m_vecOpenImGuies.begin(); iter != m_vecOpenImGuies.end();)
        {
            if (false == (*iter)->IsOpen()) {
                (*iter)->CloseImGui();
                iter = m_vecOpenImGuies.erase(iter);
            }
            else {
                ++iter;
            }
        }
    }
    ImGui::EndMainMenuBar();
}