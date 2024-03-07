#include "ToolDefines.h"
#include "UGameInstance.h"
#include "TNavigationView.h"
#include "UStageManager.h"
#include "UStage.h"

TNavigationView::TNavigationView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "NavigationView"),
	m_stMainDesc{},
	m_stNavigationView{},
	m_isInitSetting{ false },
	m_bNavigationModify{ false },
	m_bAllRender{ false },
	m_bRenderWireFrame{ false },
	m_bNavigationDebugColor{ false },
	m_dShowDeltaTime{ 0.0 },
	m_iCreateRegionIndex{0}
{
}

void TNavigationView::Free()
{
}

HRESULT TNavigationView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });
	m_stNavigationView = DOCKDESC("NavigationViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	return S_OK;
}

HRESULT TNavigationView::LoadResource()
{
	return E_NOTIMPL;
}

HRESULT TNavigationView::ReleaseResource()
{
	return E_NOTIMPL;
}

void TNavigationView::TickActive(const _double& _dTimeDelta)
{
}

void TNavigationView::LateTickActive(const _double& _dTimeDetla)
{
}

void TNavigationView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		NavigationView();
	}
	ImGui::End(); 
}

void TNavigationView::RenderMenu()
{
}

void TNavigationView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stNavigationView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 1.f, NULL, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stNavigationView.strName.c_str(), m_stNavigationView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TNavigationView::NavigationView()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	ImGui::Begin(m_stNavigationView.strName.c_str(), GetOpenPointer(), m_stNavigationView.imgWindowFlags);
	{
		ImGui::Checkbox("Navigation Modify", &m_bNavigationModify);
		if (true == m_bNavigationModify)
		{
			ImGui::Checkbox("Navigation_WireFrame", &m_bRenderWireFrame);
			ImGui::Checkbox("Navigation_DrawNav", &m_bNavigationDebugColor);
			ImGui::Checkbox("Navigation_AllRender", &m_bAllRender);
			ImGui::InputInt("Region Index", (_int*)&m_iCreateRegionIndex);
			spGameInstance->GetStage()->CreateRegion(m_iCreateRegionIndex);

		}
	}
	ImGui::End();
}
