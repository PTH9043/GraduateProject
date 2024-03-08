#include "ToolDefines.h"
#include "TModelView.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "TAssimpModel.h"
#include "UModel.h"
#include "UAnimModel.h"
#include "TShowAnimModelObject.h"
#include "TShowModelObject.h"
#include "UAnimation.h"
#include "TImGuiManager.h"
#include "UTransform.h"

TModelView::TModelView(CSHPTRREF<UDevice> _spDevice) :
	TImGuiView(_spDevice, "ModelView"),
	m_stMainDesc{},
	m_stModelDockDesc{},
	m_stAnimModelDockDesc{},
	m_ModelsContainer{},
	m_AnimModelContainer{},
	m_spModelFileFolder{ nullptr },
	m_spAnimModelFileFolder{ nullptr },
	m_vModelScale{},
	m_vAnimModelScale{},
	m_iFileIndex{ 0 },
	m_isAllConverter{ false },
	m_spShowAnimModelObject{ nullptr },
	m_spShowModelObject{ nullptr },
	m_isInitSetting{ false },
	m_isResetModel{ false },
	m_isResetAnimModel{ false },
	m_vModelPivotScale{},
	m_vAnimModelPivotScale{}
{
}

void TModelView::Free()
{
}

HRESULT TModelView::NativeConstruct()
{
	m_stMainDesc = MAINDESC(
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stModelDockDesc = DOCKDESC("ModelViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimModelDockDesc = DOCKDESC("AnimModelViewer", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_CentralNode);
	return S_OK;
}

HRESULT TModelView::LoadResource()
{
	m_spShowAnimModelObject = std::static_pointer_cast<TShowAnimModelObject>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_SHOWANIMMODELOBJECT));
	m_spShowModelObject = std::static_pointer_cast<TShowModelObject>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_SHOWMODELOBJECT));
	// AnimModelPivotScale
	m_vAnimModelPivotScale = m_spShowAnimModelObject->GetTransform()->GetScale();
	return S_OK;
}

HRESULT TModelView::ReleaseResource()
{
	m_AnimModelContainer.clear();
	m_ModelsContainer.clear();

	m_spModelFileFolder = nullptr;
	m_spAnimModelFileFolder = nullptr;

	m_spShowModelObject.reset();
	m_spShowAnimModelObject.reset();

	GetGameInstance()->RemoveActor(m_spShowModelObject);
	GetGameInstance()->RemoveActor(m_spShowAnimModelObject);

	ActiveResetSceneData();
	return S_OK;
}

void TModelView::TickActive(const _double& _dTimeDelta)
{
	if (true == m_isResetModel)
	{
		m_spShowModelObject->SetShowModel(nullptr);
	}
	if (true == m_isResetAnimModel)
	{
		m_spShowAnimModelObject->SetShowModel(nullptr);
	}
}

void TModelView::LateTickActive(const _double& _dTimeDetla)
{
}

void TModelView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stMainDesc.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stMainDesc.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stMainDesc.imgWindowFlags);
	{
		m_stMainDesc.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuildInitSetting();
		ImGui::DockSpace(m_stMainDesc.iDockSpaceID, ImVec2{}, m_stMainDesc.imgDockNodeFlags);
		// Show Model
		ImGui::NewLine();
		ShowModels();
		ShowAnimMoldels();

	}
	ImGui::End();
}

void TModelView::DockBuildInitSetting()
{
	RETURN_CHECK(true == m_isInitSetting, ;);
	{
		ImGuiID dock_main_id = m_stMainDesc.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stMainDesc.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stMainDesc.iDockSpaceID);
		// Docking Build 
		m_stModelDockDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.5f, NULL, &dock_main_id);
		m_stAnimModelDockDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.5f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow(m_stModelDockDesc.strName.c_str(), m_stModelDockDesc.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stAnimModelDockDesc.strName.c_str(), m_stAnimModelDockDesc.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stMainDesc.iDockSpaceID);
	}
	m_isInitSetting = true;
}

void TModelView::ShowModels()
{
	ImGui::Begin(m_stModelDockDesc.strName.c_str(), GetOpenPointer(), m_stModelDockDesc.imgWindowFlags);
	{
		ConvertModels();
		ResetModels();
		ShowModelList();
	}
	ImGui::End();
}

void TModelView::ShowAnimMoldels()
{
	ImGui::Begin(m_stAnimModelDockDesc.strName.c_str(), GetOpenPointer(), m_stAnimModelDockDesc.imgWindowFlags);
	{		
		ConvertAnimModels();
		ResetAnimModels();
		ShowAnimModelList();
	}
	ImGui::End();
}

void TModelView::ShowModelList()
{
	if (ImGui::BeginListBox("Model List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		using MODELPAIR = std::pair < _string, SHPTR<UModel>>;
		for (const MODELPAIR& Model : m_ModelsContainer)
		{
			_bool isTrue{ false };
			if (ImGui::Selectable(Model.first.c_str(), &isTrue))
			{
				// Set Show Model
				m_spShowModelObject->SetShowModel(Model.second);
			}
		}
		ImGui::EndListBox();
	}

	ImGui::DragFloat3("Model Scale", &m_vModelPivotScale.x, 0.001f);
	if (true == ImGui::Button("Chage View Model Scale"))
	{
		m_spShowModelObject->GetTransform()->SetScale(m_vModelPivotScale);
	}
}

void TModelView::ShowAnimModelList()
{
	if (ImGui::BeginListBox("AnimModel List", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		using ANIMMODELPAIR = std::pair < _string, SHPTR<UAnimModel>>;
		for (const ANIMMODELPAIR& Model : m_AnimModelContainer)
		{
			_bool isTrue{ false };
			if (ImGui::Selectable(Model.first.c_str(), &isTrue))
			{
				// Set Show Model
				m_spShowAnimModelObject->SetShowModel(Model.second);
			}
		}
		ImGui::EndListBox();
	}
	ImGui::NewLine();
	if (ImGui::BeginListBox("Select Animation", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		if (nullptr != m_spShowAnimModelObject)
		{
			CSHPTRREF<UAnimModel> spModel = m_spShowAnimModelObject->GetAnimModel();
			if (nullptr != spModel)
			{
				for (auto& Animation : spModel->GetAnimations())
				{
					_string strName = UMethod::ConvertWToS(Animation->GetAnimName());
					_bool isTrue{ false };
					if (ImGui::Selectable(strName.c_str(), &isTrue))
					{
						spModel->SetAnimation(Animation->GetAnimName());
					}
				}
			}
		}
		ImGui::EndListBox();
	}

	ImGui::DragFloat3("AnimModel Scale", &m_vAnimModelPivotScale.x, 0.001f);
	if (true == ImGui::Button("Chage View AnimModel Scale"))
	{
		m_spShowAnimModelObject->GetTransform()->SetScale(m_vAnimModelPivotScale);
	}
}

void TModelView::ConvertModels()
{
	if (ImGui::Button("ConvertModels"))
	{
		SHPTR<FILEGROUP> ModelFolder = GetGameInstance()->FindFolder(L"Model");
		// Folders 
		for (const FOLDERPAIR& Folder : ModelFolder->UnderFileGroupList)
		{
			LoadAssimpModelDatas(Folder.second);
		}
		// Load 
		GetGameInstance()->LoadFirstFolder(FIRST_RESOURCE_FOLDER);
		m_spModelFileFolder = GetGameInstance()->FindFolder(L"Model");
	}
}

void TModelView::ConvertAnimModels()
{
	if (ImGui::Button("ConvertAnimModels"))
	{
		SHPTR<FILEGROUP> ModelFolder = GetGameInstance()->FindFolder(L"AnimModel");
		m_AnimModelContainer.clear();
		// Folders 
		for (const FOLDERPAIR& Folder : ModelFolder->UnderFileGroupList)
		{
			LoadAssimpAnimModelDatas(Folder.second);
		}
		// Load 
		GetGameInstance()->LoadFirstFolder(FIRST_RESOURCE_FOLDER);
		m_spAnimModelFileFolder = GetGameInstance()->FindFolder(L"AnimModel");
	}
}

void TModelView::LoadAssimpModelDatas(CSHPTRREF<FILEGROUP> _spFolder)
{
	RETURN_CHECK(nullptr == _spFolder, ;);
	SHPTR<FILEGROUP> ConvertFolder = _spFolder->FindGroup(L"Convert");
	if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
	{
		for (const FILEPAIR& File : ConvertFolder->FileDataList)
		{
			SHPTR<UModel> spModel = CreateConstructorNativeNotMsg<UModel>(GetDevice(), File.second->wstrfilePath);
			m_ModelsContainer.insert(std::pair<_string, SHPTR<UModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
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
				m_ModelsContainer.insert(std::pair<_string, SHPTR<UModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
			}
		}
		// Folders 
		for (const FOLDERPAIR& Folder : _spFolder->UnderFileGroupList)
		{
			LoadAssimpModelDatas(Folder.second);
		}
	}
}

void TModelView::LoadAssimpAnimModelDatas(CSHPTRREF<FILEGROUP> _spFolder)
{
	RETURN_CHECK(nullptr == _spFolder, ;);
	RETURN_CHECK(_spFolder->wstrFolderName == L"Anim", ;);
	SHPTR<FILEGROUP> ConvertFolder = _spFolder->FindGroup(L"Convert");
	if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
	{
		// Convert Folder가 존재하는 경우 다시 만들지 않음 
		for (const FILEPAIR& File : ConvertFolder->FileDataList)
		{
			SHPTR<UAnimModel> spModel = CreateConstructorNativeNotMsg<UAnimModel>(GetDevice(), File.second->wstrfilePath);
			m_AnimModelContainer.insert(std::pair<_string, SHPTR<UAnimModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
		}
	}
	else
	{
		SHPTR<FILEGROUP> spAnimFBX = _spFolder->FindGroup(L"AnimFBX");
		SHPTR<FILEGROUP> spAnim = _spFolder->FindGroup(L"Anim");
		// FileData
		for (const FILEPAIR& File : _spFolder->FileDataList)
		{
			// Assimp 
			SHPTR<TAssimpModel> spAssimpModel = CreateConstructorNativeNotMsg<TAssimpModel>(GetDevice(),
				TAssimpModel::TYPE::ANIM, _spFolder, File.second);

			if (nullptr != spAnimFBX)
			{
				for (auto& iter : spAnimFBX->FileDataList)
				{
					spAssimpModel->LoadAnimationFBX(iter.second->wstrfilePath);
				}
			}
			if (nullptr != spAnim)
			{
				for (auto& iter : spAnim->FileDataList)
				{
					spAssimpModel->LoadAnimation(iter.second->wstrfilePath);
				}
			}


			// Save Anim Model
			if (nullptr != spAssimpModel)
			{
				_wstring wstrPath;
				spAssimpModel->SaveAnimModel(_spFolder->wstrPath, wstrPath);
				SHPTR<UAnimModel> spModel = CreateConstructorNativeNotMsg<UAnimModel>(GetDevice(), wstrPath);

				m_AnimModelContainer.insert(std::pair<_string, SHPTR<UAnimModel>>(UMethod::ConvertWToS(File.second->wstrfileName), spModel));
			}
		}
		// Folders 
		for (const FOLDERPAIR& Folder : _spFolder->UnderFileGroupList)
		{
			LoadAssimpAnimModelDatas(Folder.second);
		}
	}
}

void TModelView::ResetModels()
{
	ImGui::SameLine();
	m_isResetModel = ImGui::Button("Reset Models");
	if (true == m_isResetModel)
	{
		m_ModelsContainer.clear();
		m_spModelFileFolder = nullptr;
	}
}

void TModelView::ResetAnimModels()
{
	ImGui::SameLine();
	m_isResetAnimModel = ImGui::Button("Reset AnimModels");
	if (true == m_isResetAnimModel)
	{
		m_AnimModelContainer.clear();
		m_spAnimModelFileFolder = nullptr;
	}
}
