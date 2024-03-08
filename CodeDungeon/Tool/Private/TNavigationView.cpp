#include "ToolDefines.h"
#include "UGameInstance.h"
#include "TNavigationView.h"
#include "UStageManager.h"
#include "UStage.h"
#include "UInputManager.h"
#include "UPicking.h"
#include "UNavigation.h"
#include "URegion.h"

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
	m_iCreateRegionIndex{0},
	m_iRegionIndex{ 0 },
	m_spStageManager{nullptr},
	m_vecPosList{}
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

	m_spStageManager = Create<UStageManager>();
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

void TNavigationView::ModifyNavigation(CSHPTRREF<URegion> _spRegion)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	if (nullptr == _spRegion)
		return;

	_float2 MousePos = spGameInstance->GetMousePosition();
	if (MousePos.x > 0 && MousePos.x < WINDOW_WIDTH)
	{
		if (MousePos.y > 0 && MousePos.y < WINDOW_HEIGHT)
		{
			_float3 v3Pos;
			if (true == spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L))
			{
				SHPTR<UNavigation> _spNav = _spRegion->GetNavigation();
				m_spStageManager->GetStage()->Is_Picking(m_iRegionIndex);
				PICKINGDESC tDesc = spGameInstance->GetPickDesc();

				if (nullptr == tDesc.spActor)
					return;
	

				v3Pos = tDesc.vPickPos;
				CELLCONTAINER Cells;

				if (nullptr != _spNav)
					Cells = *_spNav->GetCells();

				for (auto& iter : Cells)
				{
					for (_uint i = 0; i < UCell::POINT_END; ++i)
					{
						_float3 Point = iter->GetPoint((UCell::POINT)i);

						if (v3Pos.y >= Point.y - 0.1f && v3Pos.y <= Point.y + 0.1f)
						{
							if (v3Pos.x >= Point.x - 0.5f && v3Pos.x <= Point.x + 0.5f)
							{
								if (v3Pos.z >= Point.z - 0.5f && v3Pos.z <= Point.z + 0.5f)
								{
									v3Pos = Point;
								}
							}
						}
					}
				}
				m_vecPosList.push_back(v3Pos);

		/*		if (2 >= (_uint)m_vecPosList.size())
				{
					m_pCubePosArr[m_iSelIndex++]->Set_Position(XMLoadFloat3(&v3Pos));
					m_bSelEnd = true;
				}
				else
				{
					_float3 vPos[3] = { m_vecPosList[SEL_1], m_vecPosList[SEL_2], m_vecPosList[SEL_3] };
					_pStage->Add_Cell(CCell::Create(m_pDevice, m_pContext, vPos, ++m_iCellIndex));
					m_vecPosList.clear();
					m_iSelIndex = SEL_1;
				}*/

			}
		}
	}
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
			m_spStageManager->GetStage()->CreateRegion(m_iCreateRegionIndex);
			
			SHPTR<REGIONLIST> Regionlist = m_spStageManager->GetStage()->GetRegionList();

			if ((*Regionlist.get()).size() > 0)
			{
				_uint iSelect = m_spStageManager->GetStage()->SelectRegion();
				if (INVALID_MINUS_STAGEVALUE != iSelect)
					m_iRegionIndex = iSelect;
				m_spStageManager->GetStage()->Delete_Region(m_iRegionIndex);
				ModifyNavigation(m_spStageManager->GetStage()->GetRegion(m_iRegionIndex));
				m_spStageManager->GetStage()->Control_Collider(m_iRegionIndex);
				m_spStageManager->GetStage()->ModifyCells(m_iRegionIndex);
				m_spStageManager->GetStage()->ShowCells(m_iRegionIndex);
			}
		}
		ImGui::End();
	}
}



	