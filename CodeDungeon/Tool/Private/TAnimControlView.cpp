#include "ToolDefines.h"
#include "TAnimControlView.h"
#include "TAnimControlModel.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "TAssimpModel.h"
#include "UAnimModel.h"

TAnimControlView::TAnimControlView(CSHPTRREF<UDevice> _spDevice) :
    TImGuiView(_spDevice, "AnimControlView"), 
	m_stMainDesc{},
	m_isInitSetting{false}, 
	m_spSelectAnimFileData{nullptr}
{
}

void TAnimControlView::Free()
{
}

HRESULT TAnimControlView::NativeConstruct()
{
    m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground, ImGuiDockNodeFlags_None,
        ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stAnimModelSelectDesc = DOCKDESC("AnimModelSelect", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimControlDesc = DOCKDESC("AnimControlViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimModifyDesc = DOCKDESC("AnimModifyViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);

	m_spAnimControlModel = std::static_pointer_cast<TAnimControlModel>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_ANIMCONTROLMODELOBJECT));
    return S_OK;
}

HRESULT TAnimControlView::LoadResource()
{
	m_spAnimControlModel->SetActive(true);
	SHPTR<FILEGROUP> AnimModels  = GetGameInstance()->FindFolder(L"AnimModel");
	for (auto& iter : AnimModels->UnderFileGroupList)
	{
		// FBX 라는 하위 폴더를 찾는다. 
		SHPTR<FILEGROUP> spFolder = iter.second->FindGroup(L"FBX");
		if (nullptr == spFolder)
		{
			ASSERT_CRASH("Plz Make FBX Folder");
		}
		// FBX에서 Convert 라는 하위 폴더를 찾는다. 
		SHPTR<FILEGROUP> spConvert = spFolder->FindGroup(L"Convert");
		if (nullptr == spConvert)
		{
			ASSERT_CRASH("Plz Make Convert Folder");
		}
		for (auto& FileData : spConvert->FileDataList)
		{
			m_AnimFileContainer.insert(MakePair(UMethod::ConvertWToS(FileData.first), FileData.second));
		}
	}
    return S_OK;
}

HRESULT TAnimControlView::ReleaseResource()
{
	m_AnimFileContainer.clear();
	m_spAnimControlModel->SetActive(false);
    return S_OK;
}

void TAnimControlView::TickActive(const _double& _dTimeDelta)
{
}

void TAnimControlView::LateTickActive(const _double& _dTimeDetla)
{
}

void TAnimControlView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		AnimModelSelectView();
		AnimControlView();
		AnimModifyView();
	}
	ImGui::End();
}

void TAnimControlView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
	    m_stAnimModelSelectDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.8f, NULL, &dock_main_id);
		m_stAnimControlDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, &m_stAnimModelSelectDesc.iDockSpaceID, &dock_main_id);
		m_stAnimModifyDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, &m_stAnimControlDesc.iDockSpaceID, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stAnimModelSelectDesc.strName.c_str(), m_stAnimModelSelectDesc.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stAnimControlDesc.strName.c_str(), m_stAnimControlDesc.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stAnimModifyDesc.strName.c_str(), m_stAnimModifyDesc.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TAnimControlView::AnimModelSelectView()
{
	ImGui::Begin(m_stAnimModelSelectDesc.strName.c_str(), GetOpenPointer(), m_stAnimModelSelectDesc.imgWindowFlags);
	{
		ImGui::Text("AnimModelSelect");
		if (ImGui::BeginListBox("AnimModel List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			using ANIMFILEDATA = std::pair < _string, SHPTR<FILEDATA>>;
			for (const ANIMFILEDATA& AnimModel : m_AnimFileContainer)
			{
				if (ImGui::Selectable(AnimModel.first.c_str()))
				{
					m_spSelectAnimFileData = AnimModel.second;
				 }
			}
			ImGui::EndListBox();
		}
		if (true == ImGui::Button("Load AnimationModel"))
		{
			// Select Folder 
			if (nullptr != m_spSelectAnimFileData)
			{
				m_spShowAnimModel = CreateConstructorNative<UAnimModel>(GetDevice(), m_spSelectAnimFileData->wstrfilePath);
				m_spAnimControlModel->SetShowModel(m_spShowAnimModel);
			}
		}
		if (nullptr == m_spSelectAnimFileData)
		{
			ImGui::Text("Plz Select AnimationModel");
		}
	}
	ImGui::End();
}

void TAnimControlView::AnimControlView()
{
	ImGui::Begin(m_stAnimControlDesc.strName.c_str(), GetOpenPointer(), m_stAnimControlDesc.imgWindowFlags);
	{
		// select Animation
		m_spAnimControlModel->AnimControlView();
	}
	ImGui::End();
}

void TAnimControlView::AnimModifyView()
{
	ImGui::Begin(m_stAnimModifyDesc.strName.c_str(), GetOpenPointer(), m_stAnimModifyDesc.imgWindowFlags);
	{
	}
	ImGui::End();
}
