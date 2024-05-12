#include "ToolDefines.h"
#include "TMapView.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "UPawn.h"
#include "TAssimpModel.h"
#include "UModel.h"
#include "TShowModelObject.h"
#include "TImGuiManager.h"
#include "UTransform.h"
#include "UPicking.h"
#include "UVIBuffer.h"
#include "UMeshContainer.h"
#include "UCollider.h"


TMapView::TMapView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "MapView"),
	m_stMainDesc{},
	m_stMapView{},

	m_dShowDeltaTime{},
	m_isInitSetting{},
	m_bRenderColliders{false},

	m_MapModelContainer{},
	m_ShowMapModelContainer{},
	m_iModelSuffix{},

	m_spSelectedModel{ nullptr },
	m_SelectedModelName{},
	m_DeleteModelsContainer{},
	m_bAddtoPicking{false}
{
}

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
	m_MapModelContainer.clear();
	m_ShowMapModelContainer.clear();
	

	return S_OK;
}

void TMapView::TickActive(const _double& _dTimeDelta)
{
	//for (auto& model : m_DeleteModelsContainer)
	//{
	//	auto it = m_ShowMapModelContainer.find(model);

	//	if (it != m_ShowMapModelContainer.end())
	//	{
	//		m_ShowMapModelContainer.erase(it);
	//	}
	//}
	//m_DeleteModelsContainer.clear();
}

void TMapView::LateTickActive(const _double& _dTimeDetla)
{
	if(m_bAddtoPicking)
	{
		for (auto& ShowModel : m_ShowMapModelContainer)
		{
			//픽킹이 가능하도록 픽킹목록에 추가
			SHPTR<UModel> spModel = ShowModel.second->GetShowModel();
			for (auto& Mesh : spModel->GetMeshContainers())
			{
				SHPTR<UVIBuffer> spVIbuffer = static_pointer_cast<UVIBuffer>(Mesh);
				SHPTR<UPawn> pawnModel = static_pointer_cast<UPawn>(ShowModel.second);
				GetGameInstance()->AddPickingObject(pawnModel, spVIbuffer);
			}
		}
	}

	if (m_bRenderColliders)
	{
		for (auto& ShowModel : m_ShowMapModelContainer)
		{
			for (auto& Colliders : ShowModel.second->GetColliderContainer())
				Colliders.second->AddRenderer(RENDERID::RI_NONALPHA_LAST);
		}
	}
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
	ImGui::Begin(m_stMapView.strName.c_str(), GetOpenPointer(), m_stMapView.imgWindowFlags);
	{
		if (ImGui::TreeNodeEx("ShowMap", ImGuiTreeNodeFlags_DefaultOpen))
		{
			LoadMapModels();
			ShowModelList();
			MouseInput();
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void TMapView::LoadAssimpModelDatas(CSHPTRREF<FILEGROUP> _spFolder)
{
	RETURN_CHECK(nullptr == _spFolder, ;);
	SHPTR<FILEGROUP> ConvertFolder = _spFolder->FindGroup(L"Convert");
	if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
	{
		for (const FILEPAIR& File : ConvertFolder->FileDataList)
		{
			SHPTR<UModel> spModel = CreateConstructorNativeNotMsg<UModel>(GetDevice(), File.second->wstrfilePath);
			m_MapModelContainer.insert(std::pair<_string, SHPTR<UModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
		}
	}
	else
	{
		// FileData
		for (const FILEPAIR& File : _spFolder->FileDataList)
		{
			// Assimp 
			SHPTR<TAssimpModel> spAssimpModel = CreateConstructorNativeNotMsg<TAssimpModel>(GetDevice(),
				TAssimpModel::TYPE::NONANIM, _spFolder, File.second);

			// Save Non Anim Model
			if (nullptr != spAssimpModel)
			{
				_wstring wstrPath;
				spAssimpModel->SaveNonAnimModel(_spFolder->wstrPath, wstrPath);
				SHPTR<UModel> spModel = CreateConstructorNativeNotMsg<UModel>(GetDevice(), wstrPath);
				m_MapModelContainer.insert(std::pair<_string, SHPTR<UModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
			}
		}
		// Folders 
		for (const FOLDERPAIR& Folder : _spFolder->UnderFileGroupList)
		{
			LoadAssimpModelDatas(Folder.second);
		}
	}
}

void TMapView::LoadMapModels()
{
	if (ImGui::Button("ConvertMap"))
	{
		SHPTR<FILEGROUP> MapFolder = GetGameInstance()->FindFolder(L"Map");
		// Folders 
		for (const FOLDERPAIR& Folder : MapFolder->UnderFileGroupList)
		{
			LoadAssimpModelDatas(Folder.second);
		}
	}
}

void TMapView::ShowModelList()
{
	if (ImGui::TreeNodeEx("Add ShowModel", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginListBox("Model List to Add", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			using MODELPAIR = std::pair < _string, SHPTR<UModel>>;
			for (const MODELPAIR& Model : m_MapModelContainer)
			{
				_bool isTrue{ false };
				if (ImGui::Selectable(Model.first.c_str(), &isTrue))
				{
					//numStr를 활용하여 중복된 모델 구분
					_string uniqueName = Model.first;
					_string numStr = std::to_string(m_iModelSuffix);
					while (m_ShowMapModelContainer.find(uniqueName) != m_ShowMapModelContainer.end())
						uniqueName.append(numStr);

					//ShowModel을 컨테이너에 추가
					SHPTR<TShowModelObject> newModel = std::static_pointer_cast<TShowModelObject>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_SHOWMODELOBJECT));
					newModel->SetShowModel(Model.second);

					for (auto& Containers : newModel->GetColliderContainer())
					{
						Containers.second->SetTranslate(newModel->GetShowModel()->GetCenterPos());
						Containers.second->SetScaleToFitModel(newModel->GetShowModel()->GetMinVertexPos(), newModel->GetShowModel()->GetMaxVertexPos());
						Containers.second->SetTransform(newModel->GetTransform());
					}

					m_ShowMapModelContainer.emplace(uniqueName, newModel);

					// numStr을 다시 제거
					size_t pos = uniqueName.find(numStr);
					if (pos != _wstring::npos)
						uniqueName.erase(pos, numStr.length());
					m_iModelSuffix++;
				}
			}
			ImGui::EndListBox();
		}
		if (ImGui::Button("Load All Map"))
		{
			using MODELPAIR = std::pair < _string, SHPTR<UModel>>;
			for (const MODELPAIR& Model : m_MapModelContainer)
			{
				//numStr를 활용하여 중복된 모델 구분
				_string uniqueName = Model.first;
				_string numStr = std::to_string(m_iModelSuffix);
				while (m_ShowMapModelContainer.find(uniqueName) != m_ShowMapModelContainer.end())
					uniqueName.append(numStr);

				//ShowModel을 컨테이너에 추가
				SHPTR<TShowModelObject> newModel = std::static_pointer_cast<TShowModelObject>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_SHOWMODELOBJECT));
				newModel->SetShowModel(Model.second);

				for (auto& Containers : newModel->GetColliderContainer())
				{
					Containers.second->SetTranslate(newModel->GetShowModel()->GetCenterPos());
					Containers.second->SetScaleToFitModel(newModel->GetShowModel()->GetMinVertexPos(), newModel->GetShowModel()->GetMaxVertexPos());
					Containers.second->SetTransform(newModel->GetTransform());
				}

				m_ShowMapModelContainer.emplace(uniqueName, newModel);

				// numStr을 다시 제거
				size_t pos = uniqueName.find(numStr);
				if (pos != _wstring::npos)
					uniqueName.erase(pos, numStr.length());
				m_iModelSuffix++;
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Current ShowModels", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginListBox("Current Models Shown", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto& Model : m_ShowMapModelContainer)
			{
				_bool isTrue{ false };
				if (ImGui::Selectable(Model.first.c_str(), &isTrue))
				{
					m_spSelectedModel = static_pointer_cast<UPawn>(Model.second);
					m_SelectedModelName = Model.first;
				}
			}
			ImGui::EndListBox();
		}
		ImGui::Checkbox("Add Maps To Picking", &m_bAddtoPicking);
			
		/*if (ImGui::Button("Clear Current ShowModels"))
			ClearCurrentModel();
		if (ImGui::Button("Clear All"))
			ClearAllShowModels();*/

		if (m_spSelectedModel != nullptr)
		{
			ImGui::Text("Selected Model: %s", m_SelectedModelName.c_str());
		}
		else
			ImGui::Text("Selected Model: None, Select Current Model");

		ImGui::Checkbox("Render_Colliders", &m_bRenderColliders);
			
		ImGui::TreePop();

	}
}

//void TMapView::ClearCurrentModel()
//{
//	auto it = m_ShowMapModelContainer.begin();
//	while (it != m_ShowMapModelContainer.end())
//	{
//		if (it->second == m_spSelectedModel)
//		{
//			GetGameInstance()->RemoveActor(it->second);
//			m_DeleteModelsContainer.push_back(it->first);
//			break;
//		}
//		else
//		{
//			++it;
//		}
//	}
//	m_spSelectedModel.reset();
//}
//
//void TMapView::ClearAllShowModels()
//{
//	auto it = m_ShowMapModelContainer.begin();
//	while (it != m_ShowMapModelContainer.end())
//	{
//		GetGameInstance()->RemoveActor(it->second);
//		m_DeleteModelsContainer.push_back(it->first);
//		it++;
//	}
//	if (m_spSelectedModel != nullptr)
//		m_spSelectedModel.reset();
//}

void TMapView::MouseInput()
{
	if(m_bAddtoPicking)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		_float2 MousePos = spGameInstance->GetMousePosition();

		if (MousePos.x <= 0 || MousePos.x >= WINDOW_WIDTH)
			return;

		if (MousePos.y <= 0 || MousePos.y >= WINDOW_HEIGHT)
			return;

		if (!spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L))
			return;

		PICKINGDESC tDesc = spGameInstance->GetPickDesc();
		if (!tDesc.bPickingSuccess)
			return;

		for (auto& ShowModel : m_ShowMapModelContainer)
		{
			if (ShowModel.second == tDesc.spPawn)
			{
				m_spSelectedModel = tDesc.spPawn;
				m_SelectedModelName = ShowModel.first;
				return;
			}
		}
	}
}