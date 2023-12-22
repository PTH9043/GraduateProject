#include "ToolDefines.h"
#include "TSceneView.h"
#include "USkyBox.h"
#include "UGameInstance.h"
#include "UActorGroup.h"
#include "UMethod.h"
#include "UTransform.h"
#include "UTexGroup.h"
#include "UTexture.h"
#include "UVIBufferTerrain.h"
#include "TShowTerrain.h"
#include "TMainScene.h"
#include "TImGuiManager.h"

TSceneView::TSceneView(CSHPTRREF<UDevice> _spDevice) : 
	TImGuiView(_spDevice, "SceneView"),
	m_stSceneView{},
	m_stHierackyView{},
	m_stPerspeciveView{},
	m_stSkyBoxView{},
	m_isInitSetting{ false },
	m_arrViewMenus{},
	m_spSkyBox{ nullptr },
	m_conunomapGameLayerMap{},
	m_spTerrainFolder{ nullptr },
	m_spTerrainHeightMapFolder{ nullptr },
	m_fHeightMapSize{0},
	m_spShowTerrain{nullptr},
	m_spSelectedTerrainFile{nullptr}
{
}

void TSceneView::Free()
{
}

HRESULT TSceneView::NativeConstruct()
{
	m_stSceneView = MAINDESC(
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse, ImGuiDockNodeFlags_None,
		ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stHierackyView = DOCKDESC("HierarckyView", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_NoCentralNode);
	m_stPerspeciveView = DOCKDESC("PersepectiveView", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_NoCentralNode);
	m_stSkyBoxView = DOCKDESC("SkyBoxView", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_NoCentralNode);
	m_stTerrainView = DOCKDESC("TerrainView", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_NoCentralNode);
	m_stModelView = DOCKDESC("ModelView", ImGuiWindowFlags_NoFocusOnAppearing,
		ImGuiDockNodeFlags_NoCentralNode);

	for (auto& iter : m_arrViewMenus)
	{
		iter = false;
	}
	return S_OK;
}

HRESULT TSceneView::LoadResource()
{
	SHPTR<UGameInstance> spGameInstance = GetGameInstance();
	m_spShowTerrain = std::static_pointer_cast<TShowTerrain>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SHOWTERRAIN));
	// Create SkyBox
	USkyBox::SKYBOXDESC tDesc{L"SkyBox", L"SkyBox_Cloud", true};
	VOIDDATAS vecDatas;
	vecDatas.push_back(&tDesc);
	m_spSkyBox = std::static_pointer_cast<USkyBox>(spGameInstance->CloneActorAdd(PROTO_ACTOR_SKYBOX, vecDatas));
	// Create Terrain
	{
		m_spTerrainFolder = spGameInstance->FindFolder(L"Terrain");
		if (nullptr != m_spTerrainFolder)
		{
			m_spTerrainHeightMapFolder = m_spTerrainFolder->FindUnder(L"HeightMap");
		}
	}
	UpdateContainers();
	return S_OK;
}

HRESULT TSceneView::ReleaseResource()
{
	m_spShowTerrain.reset();
	m_spSkyBox.reset();
	m_conunomapGameLayerMap.clear();
	SHPTR< TImGuiManager> spImguiManager = GET_INSTANCE(TImGuiManager);
	spImguiManager->SetResetScene(true);
	return S_OK;
}

void TSceneView::TickActive(const _double& _dTimeDelta)
{
}

void TSceneView::LateTickActive(const _double& _dTimeDetla)
{
}

void TSceneView::RenderActive()
{
	ImGui::SetNextWindowPos(m_stSceneView.vPos, ImGuiCond_Appearing);
	ImGui::SetNextWindowSize(m_stSceneView.vSize, ImGuiCond_Appearing);
	// Name, OpenPointer, MainDesc 
	ImGui::Begin(GetName().c_str(), GetOpenPointer(), m_stSceneView.imgWindowFlags);
	{
		m_stSceneView.iDockSpaceID = ImGui::GetID(GetName().c_str());
		DockBuilding();
		ImGui::DockSpace(m_stSceneView.iDockSpaceID, ImVec2{}, m_stSceneView.imgDockNodeFlags);

		if(ImGui::BeginMenuBar())
		{
			static const char* MENUNAME{ "OpenView"};
			if (ImGui::BeginMenu(MENUNAME))
			{
				static const char* MENU_VIEW_NAME_TERRAIN{ "TerrainView" };
				if (ImGui::MenuItem(MENU_VIEW_NAME_TERRAIN, "", m_arrViewMenus[OPENVIEW::VIEW_TERRAIN]))
				{
					m_arrViewMenus[OPENVIEW::VIEW_TERRAIN] = !m_arrViewMenus[OPENVIEW::VIEW_TERRAIN];
				}
				static const char* MENU_VIEW_NAME_SKYBOX{ "SkyBoxView" };
				if (ImGui::MenuItem(MENU_VIEW_NAME_SKYBOX, "", m_arrViewMenus[OPENVIEW::VIEW_SKYBOX]))
				{
					m_arrViewMenus[OPENVIEW::VIEW_SKYBOX] = !m_arrViewMenus[OPENVIEW::VIEW_SKYBOX];
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ShowHieracky();
		ShowPerspective();
		ShowTerrainView();
	}
	ImGui::End();
}

void TSceneView::ShowHieracky()
{
	if(ImGui::Begin(m_stHierackyView.strName.c_str(), GetOpenPointer(), m_stHierackyView.imgWindowFlags))
	{

	}
	ImGui::End();
}

void TSceneView::ShowPerspective()
{
	if(ImGui::Begin(m_stPerspeciveView.strName.c_str(), GetOpenPointer(), m_stPerspeciveView.imgWindowFlags))
	{
		if (true == IsObjectCountChange())
		{
			UpdateContainers();
		}

		for (const auto& LayerContainer : m_conunomapGameLayerMap)
		{
			if (true == ImGui::TreeNode(LayerContainer.first.c_str()))
			{
				for (const auto& GameObjectContainer : LayerContainer.second)
				{
					if (true == ImGui::TreeNode(GameObjectContainer.first.c_str()))
					{
						int i = 1;
						for (const SHPTR<UActor>& GameObject : GameObjectContainer.second)
						{
							_string str = GameObjectContainer.first + " (" + std::to_string(i++) + ")";
							if (true == ImGui::TreeNode(str.c_str()))
							{
								GameObject->ShowObjectInfo();
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

void TSceneView::ShowTerrainView()
{
	if (false == m_arrViewMenus[OPENVIEW::VIEW_TERRAIN])
		return;

	if (ImGui::Begin(m_stTerrainView.strName.c_str(), GetOpenPointer(), m_stTerrainView.imgWindowFlags))
	{
		if (nullptr != m_spTerrainHeightMapFolder)
		{
			if (ImGui::TreeNode(UMethod::ConvertWToS(m_spTerrainHeightMapFolder->wstrFolderName).c_str()))
			{
				ImGui::Text("Terrain HeightMap List");
				if (ImGui::BeginListBox("Select Terrain HeightMap", ImVec2(-FLT_MIN, 7 * ImGui::GetTextLineHeightWithSpacing())))
				{
					for (auto& Textures : m_spTerrainHeightMapFolder->FileDataList)
					{
						if (true == ImGui::Selectable(UMethod::ConvertWToS(Textures.first).c_str()))
						{
							m_spSelectedTerrainFile = Textures.second;
						}
					}
				}
				ImGui::TreePop();
			}
			if (nullptr != m_spSelectedTerrainFile)
			{
				ImGui::Text(UMethod::ConvertWToS(m_spSelectedTerrainFile->wstrfileName).c_str());
				ImGui::DragFloat("HeightMapSize", &m_fHeightMapSize, 0.1f);
				if (true == ImGui::Button("HeightMapLoad"))
				{
					SHPTR<UVIBufferTerrain>		spTerrain = CreateConstructorNative<UVIBufferTerrain>(GetDevice(), m_spSelectedTerrainFile->wstrfilePath, m_fHeightMapSize);
					m_spShowTerrain->SetTerrrainBuffer(spTerrain);

				}
			}
		}
	}
	ImGui::End();
}

void TSceneView::DockBuilding()
{
	if (false == m_isInitSetting)
	{
		ImGuiID dock_main_id = m_stSceneView.iDockSpaceID;
		ImGui::DockBuilderRemoveNode(m_stSceneView.iDockSpaceID);
		ImGui::DockBuilderAddNode(m_stSceneView.iDockSpaceID);
		// Docking Build 
		m_stHierackyView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.5f, NULL, &dock_main_id);
		m_stPerspeciveView.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.5f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow(m_stHierackyView.strName.c_str(), m_stHierackyView.iDockSpaceID);
		ImGui::DockBuilderDockWindow(m_stPerspeciveView.strName.c_str(), m_stPerspeciveView.iDockSpaceID);
		ImGui::DockBuilderFinish(m_stSceneView.iDockSpaceID);

		m_isInitSetting = true;
	}
}

void TSceneView::UpdateContainers()
{
	SHPTR<UGameInstance> spGameInstance = GetGameInstance();
	m_conunomapGameLayerMap.clear();
	for (auto& iter : spGameInstance->GetClonesArr())
	{
		for (auto& Layer : iter)
		{
			GAMEOBJECTCONTAINERS* pContainers{ nullptr };
			_string LayerString = UMethod::ConvertWToS(Layer.first);
			const auto& findIter = m_conunomapGameLayerMap.find(LayerString);
			if (findIter == m_conunomapGameLayerMap.end())
			{
				GAMEOBJECTCONTAINERS Containers;
				m_conunomapGameLayerMap.insert(std::make_pair(LayerString, Containers));
				pContainers = &m_conunomapGameLayerMap.find(LayerString)->second;
			}
			else
			{
				pContainers = &findIter->second;
			}

			for (const auto& GameObject : Layer.second->GetActiveActorList())
			{
				UActor* pActor = GameObject.get();
				_string strObjectName = typeid(*pActor).name();
				size_t iIndex = strObjectName.find("::");
				strObjectName = strObjectName.substr(iIndex + 2, strObjectName.size());
				if (pContainers->size() > 0)
				{
					const auto& FindGameObjectsIter = pContainers->find(strObjectName);
					if (pContainers->end() == FindGameObjectsIter)
					{
						GAMEOBJECTLIST Containers;
						Containers.push_back(GameObject);
						pContainers->insert(std::make_pair(strObjectName, Containers));
					}
					else
					{
						FindGameObjectsIter->second.push_back(GameObject);
					}
				}
				else
				{
					GAMEOBJECTLIST Containers;
					Containers.push_back(GameObject);
					pContainers->insert(std::make_pair(strObjectName, Containers));
				}
			}
			m_iGameObjectCnt += (_uint)(Layer.second->GetActiveActorList().size());
		}
	}
}

bool TSceneView::IsObjectCountChange()
{
	size_t sizeCnt{ 0 };
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	for (auto& iter : spGameInstance->GetClonesArr())
	{
		for (auto& Layer : iter)
		{
			sizeCnt += Layer.second->GetActiveActorList().size();
		}
	}

	if (sizeCnt != m_iGameObjectCnt)
	{
		return true;
	}
	return false;
}
