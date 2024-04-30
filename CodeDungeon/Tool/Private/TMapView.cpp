#include "ToolDefines.h"
#include "UGameInstance.h"
#include "UInputManager.h"
#include "UPicking.h"
#include "UDefaultDebugging.h"
#include "TShowModelObject.h"
#include "UTransform.h"
#include "TMapView.h"

void TMapView::Free()
{
}

HRESULT TMapView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stMapView = DOCKDESC("MapViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);

	return S_OK;
}

HRESULT TMapView::LoadResource()
{
	return E_NOTIMPL;
}

HRESULT TMapView::ReleaseResource()
{
	return E_NOTIMPL;
}

void TMapView::TickActive(const _double& _dTimeDelta)
{
}

void TMapView::LateTickActive(const _double& _dTimeDetla)
{
}

void TMapView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stMapView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, NULL, &dock_main_id);
		ImGui::DockBuilderDockWindow(m_stMapView.strName.c_str(), m_stMapView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TMapView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		MapView();

	}
	ImGui::End();
}

void TMapView::MapView()
{








}

void TMapView::LoadMapDatas()
{
	if (ImGui::TreeNodeEx("Add ShowModel", ImGuiTreeNodeFlags_DefaultOpen))
	{



	}
}

void TMapView::LoadRooms()
{
	//SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	//SHPTR<FILEGROUP> MapFolder = spGameInstance->FindFolder(L"Map");

	//SHPTR<FILEGROUP> ConvertFolder = MapFolder->FindGroup(L"Convert");
	//if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
	//{
	//	for (const FILEPAIR& File : ConvertFolder->FileDataList)
	//	{
	//		_wstring FileName = File.second->wstrfileName;
	//		size_t pos = FileName.find(L"_FBX.bin");
	//		if (pos != _wstring::npos)
	//			FileName.erase(pos, FileName.length());

	//		SHPTR<TShowModelObject> _Room = std::static_pointer_cast<TShowModelObject>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_SHOWMODELOBJECT));
	//		_Room->SetShowModel(ModelProtoTag);

	//		m_spMapModelContainer->emplace(FileName, _Room);

	//		pos = ModelProtoTag.find(FileName);
	//		if (pos != _wstring::npos)
	//			ModelProtoTag.erase(pos, FileName.length());
	//	}
	//}
	//else
	//{
	//	return;
	//}
}
