#include "ToolDefines.h"
#include "TAnimControlView.h"
#include "TAnimControlModel.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "TAssimpModel.h"
#include "UAnimModel.h"
#include "UAnimation.h"
#include "AnimOccursEvents.h"
#include "AnimSectionEvents.h"


const _char* TAnimControlView::s_AnimTags[1000]{};

TAnimControlView::TAnimControlView(CSHPTRREF<UDevice> _spDevice) :
    TImGuiView(_spDevice, "AnimControlView"), 
	m_stMainDesc{},
	m_isInitSetting{false}, 
	m_spSelectAnimFileData{nullptr},
	m_iSelectAnimEvent{0},
	m_spSelectAnim{nullptr},
	m_AnimMaxTagCount{0}
{
	
}

void TAnimControlView::Free()
{
}

HRESULT TAnimControlView::NativeConstruct()
{
    m_stMainDesc = MAINDESC(ImGuiWindowFlags_NoBackground , ImGuiDockNodeFlags_None,
        ImVec2{ (_float)WINDOW_WIDTH, 0.f }, ImVec2{ 500.f, (_float)WINDOW_HEIGHT });

	m_stAnimModelSelectDesc = DOCKDESC("AnimModelSelect", ImGuiWindowFlags_NoFocusOnAppearing ,
		ImGuiDockNodeFlags_CentralNode);
	m_stAnimModifyDesc = DOCKDESC("AnimModifyViewer", ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_HorizontalScrollbar | 
		ImGuiWindowFlags_AlwaysVerticalScrollbar,
		ImGuiDockNodeFlags_CentralNode);

	m_spAnimControlModel = std::static_pointer_cast<TAnimControlModel>(GetGameInstance()->CloneActorAdd(PROTO_ACTOR_ANIMCONTROLMODELOBJECT));
    return S_OK;
}

HRESULT TAnimControlView::LoadResource()
{
	SHPTR<FILEGROUP> AnimModels  = GetGameInstance()->FindFolder(L"AnimModel");
	for (auto& iter : AnimModels->UnderFileGroupList)
	{
		// FBX에서 Convert 라는 하위 폴더를 찾는다. 
		SHPTR<FILEGROUP> spConvert = iter.second->FindGroup(L"Convert");
		assert(nullptr != spConvert);
		m_spSelectAnimFileFolder = spConvert;
		for (auto& FileData : spConvert->FileDataList)
		{
			m_AnimFileContainer.insert(MakePair(UMethod::ConvertWToS(FileData.first), FileData.second));
		}
	}
    return S_OK;
}

HRESULT TAnimControlView::ReleaseResource()
{
	m_spSelectAnim = nullptr;
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
	    m_stAnimModelSelectDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.2f, NULL, &dock_main_id);
		m_stAnimModifyDesc.iDockSpaceID = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.8f, NULL, &dock_main_id);
		// DockBuild
		ImGui::DockBuilderDockWindow(m_stAnimModelSelectDesc.strName.c_str(), m_stAnimModelSelectDesc.iDockSpaceID);
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
				m_spAnimControlModel->SetShowModel(m_spShowAnimModel, m_spSelectAnimFileFolder);

				_int iIndex{ 0 };
				for (auto& iter : m_spAnimControlModel->GetAnimationClips())
				{
					s_AnimTags[iIndex++] = iter.first.c_str();
				}
				m_AnimMaxTagCount = iIndex;
			}
		}
		if (nullptr == m_spSelectAnimFileData)
		{
			ImGui::Text("Plz Select AnimationModel");
		}
	}
	ImGui::End();
}

void TAnimControlView::AnimModifyView()
{
	ImGui::Begin(m_stAnimModifyDesc.strName.c_str(), GetOpenPointer(), m_stAnimModifyDesc.imgWindowFlags);
	{
		// select Animation
		m_spAnimControlModel->ShowAnimModify();
		MakeAnimEvent();
	}
	ImGui::End();
}

void TAnimControlView::MakeAnimEvent()
{
	RETURN_CHECK(nullptr == m_spShowAnimModel, ;);

	static const _char* ANIMTYPETAG[]{"SECTION", "OCCURS"};

	SHPTR<UAnimation> spCurAnimation = m_spShowAnimModel->GetCurrentAnimation();

	if (ImGui::TreeNodeEx("MakeAnimEvent",ImGuiTreeNodeFlags_Bullet))
	{
		ImGui::Combo("AnimEvent", &m_iSelectAnimEvent, ANIMTYPETAG, 2);
		if (true == ImGui::Button("SelectAnim"))
		{
			m_spSelectAnim = spCurAnimation;
		}
		ImGui::SameLine();
		if (true == ImGui::Button("MakeEvent"))
		{
			ANIMEVENTTYPE Event = static_cast<ANIMEVENTTYPE>(m_iSelectAnimEvent);
			SHPTR<UAnimEvent> spAnimEvent{ nullptr };
			switch (Event)
			{
			case ANIMEVENT_CAMERA:
				break;
			case ANIMEVENT_ANIMCHANGESBETWEEN:
				spAnimEvent = Create< UAnimChangeBetweenEvent>();
				break;
			case ANIMEVENT_COLLIDER:

				break;
			case ANIMEVENT_EFFECT:

				break;
			case ANIMEVENT_OBJACTIVE:

				break;
			case ANIMEVENT_ANIMOCCURSTIMEPASS:
				spAnimEvent = Create< UAnimOccursTimePassEvent>();
				break;
			case ANIMEVENT_SOUND:
				break;
			}
			if (nullptr != m_spSelectAnim)
				m_spSelectAnim->InsertAnimEvent(Event, spAnimEvent);
		}

		if (nullptr != m_spSelectAnim)
		{
			// Options
			static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody| ImGuiTableFlags_ScrollY;

			// Event Container 
			for (auto& iter : m_spSelectAnim->GetAnimEventContainer())
			{
				switch (iter.first)
				{
				case ANIMEVENT_CAMERA:
					break;
				case ANIMEVENT_ANIMCHANGESBETWEEN:
					AnimSectionShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENT_COLLIDER:
					break;
				case ANIMEVENT_EFFECT:
					break;
				case ANIMEVENT_OBJACTIVE:
					break;
				case ANIMEVENT_ANIMOCCURSTIMEPASS:
					AnimOccursShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENT_SOUND:
					break;
				}
			}
		}
		else
		{
			ImGui::Text("Plz select Animation"); 
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimSectionShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags,  const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("AnimChangesBetweenShow", ImGuiTreeNodeFlags_Bullet))
	{
		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::BeginTable("AnimChangesBetween", 6, _flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 20), 0.0f))
		{
			ImGui::TableSetupColumn("InputTrigger", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("StartT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("EndT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextAnim", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("SupV", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextTA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			static _int iSelectAnim{ 0 };
			static _float fNextTimeAcc{ 0.f };
			static _string InputTrigger = "##InputTrigger";
			static _string StartT = "##StartT";
			static _string EndT = "##EndT";
			static _string NextAnim = "##NextAnim";
			static _string SupV = "##SupV";
			static _string NextAnimTimeAcc = "##NextAnimTimeAcc";

			_int iIndex{ 0 };
			_float Duration = static_cast<_float>(_spAnim->GetDuration());
			for (auto& iter : _AnimEvent)
			{
				_string Index = _string::to_string(iIndex++);

				 ANIMEVENTSECTIONDESC* SectionDesc = static_cast<ANIMEVENTSECTIONDESC*>(iter->OutAnimEventDesc());
				 ANIMCHANGEDESC* ChangeDesc = static_cast<ANIMCHANGEDESC*>(iter->OutOtherEventDesc());
				 {
					 ImGui::TableNextColumn();
					 ImGui::SetNextItemWidth(-FLT_MIN);
					 _string str = UMethod::ConvertWToS(SectionDesc->wstrEventTrigger);
					 ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH);
					 SectionDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
				 }
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float StartTime = static_cast<_float>(SectionDesc->dStartTime);
					ImGui::DragFloat(StartT + Index, &StartTime, 0.0f, Duration);
					SectionDesc->dStartTime = static_cast<_double>(StartTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float EndTime = static_cast<_float>(SectionDesc->dEndTime);
					ImGui::DragFloat(EndT + Index, &EndTime, 0.0f, Duration);
					SectionDesc->dEndTime = static_cast<_double>(EndTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (true == ImGui::Combo(NextAnim + Index, &iSelectAnim, &s_AnimTags[0], m_AnimMaxTagCount))
					{
						ChangeDesc->iNextAnimIndex = m_spAnimControlModel->GetAnimationClips().find(s_AnimTags[iSelectAnim])->second;
						fNextTimeAcc = static_cast<_float>(m_spShowAnimModel->GetAnimations()[ChangeDesc->iNextAnimIndex]->GetTimeAcc());
					}
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::DragFloat(SupV + Index, &ChangeDesc->fSupplyAnimValue, 0.01f, 0.01f, 10.f);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(ChangeDesc->dNextAnimTimeAcc);
					ImGui::DragFloat(NextAnimTimeAcc + Index, &Time, 0.01f, 0.01f, fNextTimeAcc);
					ChangeDesc->dNextAnimTimeAcc = static_cast<_double>(Time);
				}
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimOccursShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("AnimOccursTimePassShow", ImGuiTreeNodeFlags_Bullet))
	{
		ImGui::SetNextItemWidth(-FLT_MIN);
		if (ImGui::BeginTable("AnimOccursTimePass", 5, _flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 20), 0.0f))
		{
			ImGui::TableSetupColumn("InputTrigger", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Over", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextAnim", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("SupV", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextTA", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			static _int iSelectAnim{ 0 };
			static _float fNextTimeAcc{ 0.f };

			static _string InputTrigger = "##InputTrigger2";
			static _string OverT = "##OverT2";
			static _string NextAnim = "##NextAnim2";
			static _string SupV = "##SupV2";
			static _string NextAnimTimeAcc = "##NextAnimTimeAcc2";

			_int iIndex{ 0 };
			_float Duration = static_cast<_float>(_spAnim->GetDuration());
			for (auto& iter : _AnimEvent)
			{
				_string Index = _string::to_string(iIndex++);

				ANIMOCURRESDESC* OccursDesc = static_cast<ANIMOCURRESDESC*>(iter->OutAnimEventDesc());
				ANIMCHANGEDESC* ChangeDesc = static_cast<ANIMCHANGEDESC*>(iter->OutOtherEventDesc());

				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_string str = UMethod::ConvertWToS(OccursDesc->wstrEventTrigger);
					ImGui::InputText(InputTrigger + Index, &str[0], MAX_BUFFER_LENGTH);
					OccursDesc->wstrEventTrigger = UMethod::ConvertSToW(str);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(OccursDesc->dAnimOccursTime);
					ImGui::DragFloat(OverT + Index, &Time, 0.0f, Duration);
					OccursDesc->dAnimOccursTime = static_cast<_double>(Time);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (true == ImGui::Combo(NextAnim + Index, &iSelectAnim, &s_AnimTags[0], m_AnimMaxTagCount))
					{
						ChangeDesc->iNextAnimIndex = m_spAnimControlModel->GetAnimationClips().find(s_AnimTags[iSelectAnim])->second;
						fNextTimeAcc = static_cast<_float>(m_spShowAnimModel->GetAnimations()[ChangeDesc->iNextAnimIndex]->GetTimeAcc());
					}
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::DragFloat(SupV + Index, &ChangeDesc->fSupplyAnimValue, 0.01f, 0.01f, 10.f);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float Time = static_cast<_float>(ChangeDesc->dNextAnimTimeAcc);
					ImGui::DragFloat(NextAnimTimeAcc + Index, &Time, 0.01f, 0.01f, fNextTimeAcc);
					ChangeDesc->dNextAnimTimeAcc = static_cast<_double>(Time);
				}
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}