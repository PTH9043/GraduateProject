#include "ToolDefines.h"
#include "TAnimControlView.h"
#include "TAnimControlModel.h"
#include "UGameInstance.h"
#include "UMethod.h"
#include "TAssimpModel.h"
#include "UAnimModel.h"
#include "UAnimation.h"
#include "AnimEventParents.h"
#include "AnimSectionEvents.h"

const _char* TAnimControlView::KEYPRESSTAG[] = {
	"KEY_UP", "KEY_PRESSING",	"KEY_DOWN","EMPTY"
};

const _char* TAnimControlView::KEYTAG[]{
	"DIK_ESCAPE", "DIK_1", "DIK_2", "DIK_3", "DIK_4", "DIK_5",
	"DIK_6", "DIK_7", "DIK_8", "DIK_9", "DIK_0", "DIK_MINUS",
	"DIK_EQUALS", "DIK_BACK", "DIK_TAB", "DIK_Q", "DIK_W", "DIK_E",
	"DIK_R", "DIK_T", "DIK_Y", "DIK_U", "DIK_I", "DIK_O", "DIK_P",
	"DIK_LBRACKET", "DIK_RBRACKET", "DIK_RETURN", "DIK_LCONTROL", "DIK_A",
	"DIK_S", "DIK_D", "DIK_F", "DIK_G", "DIK_H", "DIK_J", "DIK_K",
	"DIK_L", "DIK_SEMICOLON", "DIK_APOSTROPHE", "DIK_GRAVE", "DIK_LSHIFT",
	"DIK_BACKSLASH", "DIK_Z", "DIK_X", "DIK_C", "DIK_V", "DIK_B",
	"DIK_N", "DIK_M", "DIK_COMMA", "DIK_PERIOD", "DIK_SLASH", "DIK_RSHIFT",
	"DIK_MULTIPLY", "DIK_LMENU", "DIK_SPACE", "DIK_CAPITAL", "DIK_F1", "DIK_F2",
	"DIK_F3", "DIK_F4", "DIK_F5", "DIK_F6", "DIK_F7", "DIK_F8", "DIK_F9",
	"DIK_F10", "DIK_NUMLOCK", "DIK_SCROLL", "DIK_NUMPAD7", "DIK_NUMPAD8", "DIK_NUMPAD9",
	"DIK_SUBTRACT", "DIK_NUMPAD4", "DIK_NUMPAD5", "DIK_NUMPAD6", "DIK_ADD", "DIK_NUMPAD1",
	"DIK_NUMPAD2", "DIK_NUMPAD3", "DIK_NUMPAD0", "DIK_DECIMAL", "DIK_OEM_102", "DIK_F11",
	"DIK_F12", "DIK_F13", "DIK_F14", "DIK_F15", "DIK_KANA", "DIK_ABNT_C1", "DIK_CONVERT",
	"DIK_NOCONVERT", "DIK_YEN", "DIK_ABNT_C2", "DIK_NUMPADEQUALS", "DIK_PREVTRACK", "DIK_AT",
	"DIK_COLON", "DIK_UNDERLINE", "DIK_KANJI", "DIK_STOP", "DIK_AX", "DIK_UNLABELED", "DIK_NEXTTRACK",
	"DIK_NUMPADENTER", "DIK_RCONTROL", "DIK_MUTE", "DIK_CALCULATOR", "DIK_PLAYPAUSE", "DIK_MEDIASTOP",
	"DIK_VOLUMEDOWN", "DIK_VOLUMEUP", "DIK_WEBHOME", "DIK_NUMPADCOMMA", "DIK_DIVIDE", "DIK_SYSRQ",
	"DIK_RMENU", "DIK_PAUSE", "DIK_HOME", "DIK_UP", "DIK_PRIOR", "DIK_LEFT", "DIK_RIGHT", "DIK_END",
	"DIK_DOWN", "DIK_NEXT", "DIK_INSERT", "DIK_DELETE", "DIK_LWIN", "DIK_RWIN", "DIK_APPS", "DIK_POWER",
	"DIK_SLEEP", "DIK_WAKE", "DIK_WEBSEARCH", "DIK_WEBFAVORITES", "DIK_WEBREFRESH", "DIK_WEBSTOP",
	"DIK_WEBFORWARD", "DIK_WEBBACK", "DIK_MYCOMPUTER", "DIK_MAIL", "DIK_MEDIASELECT", "EMPTY"
};

const _char* TAnimControlView::MOUSETAG[]{ "DIMB_L", "DIMB_R", "DIMB_WHEEL", "EMPTY" };

TAnimControlView::TAnimControlView(CSHPTRREF<UDevice> _spDevice) :
    TImGuiView(_spDevice, "AnimControlView"), 
	m_stMainDesc{},
	m_isInitSetting{false}, 
	m_spSelectAnimFileData{nullptr},
	m_iSelectAnimEvent{0},
	m_spSelectAnim{nullptr}
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
	m_spShowAnimModel = nullptr;
	m_spSelectAnim = nullptr;
	m_AnimFileContainer.clear();
	m_spAnimControlModel->ReleaseShowModel();
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

	static const _char* ANIMTYPETAG[]{ "EFFECT", "SOUND", "COLLIDER", "CAMERA", "OBJACTIVE", "ANIMCHANGEBETWEEN",
		"ANIMOCCURSTIMERPASS" };

	SHPTR<UAnimation> spCurAnimation = m_spShowAnimModel->GetCurrentAnimation();

	if (ImGui::TreeNodeEx("MakeAnimEvent",ImGuiTreeNodeFlags_Bullet))
	{
		if (true == ImGui::Button("SelectAnim"))
		{
			m_spSelectAnim = spCurAnimation;
		}

		if (nullptr != m_spSelectAnim)
		{
			ImGui::Combo("AnimEvent", &m_iSelectAnimEvent, ANIMTYPETAG, 7);
			if (true == ImGui::Button("MakeEvent"))
			{
				SHPTR<UAnimEvent> spAnimEvent{ nullptr };
				switch (m_iSelectAnimEvent)
				{
				case ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN:
					spAnimEvent = Create< UAnimChangeBetweenEvent>();
					break;
				case ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_CAMERA:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_COLLIDER:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_EFFECT:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_OBJACTIVE:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_SOUND:
					break;
				}
				spCurAnimation->InsertAnimEvent((ANIMEVENTTYPE)m_iSelectAnimEvent, spAnimEvent);
			}

			ImGuiTreeNodeFlags Flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_DefaultOpen;
			// Options
			static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
				| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody| ImGuiTableFlags_ScrollY;

			// Event Container 
			for (auto& iter : m_spSelectAnim->GetAnimEventContainer())
			{
				switch (iter.first)
				{
				case ANIMEVENTTYPE::ANIMEVENT_ANIMCHANGESBETWEEN:
					AnimChangesBetweenShow(m_spSelectAnim, flags, iter.second);
					break;
				case ANIMEVENTTYPE::ANIMEVENT_ANIMOCCURSTIMEPASS:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_CAMERA:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_COLLIDER:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_EFFECT:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_OBJACTIVE:
					break;
				case ANIMEVENTTYPE::ANIMEVENT_SOUND:
					break;
				}
			}
		}
		ImGui::TreePop();
	}
}

void TAnimControlView::AnimChangesBetweenShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags,  const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent)
{
	if (ImGui::TreeNodeEx("AnimChangesBetweenShow", ImGuiTreeNodeFlags_Bullet))
	{
		if (ImGui::BeginTable("AnimChangesBetween", 8, _flags, ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 20), 0.0f))
		{
			ImGui::TableSetupColumn("Idx", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Mouse", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("StartT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("EndT", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("NextAnim", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("SupV", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();

			static _int iSelectKeyPressEvent{ KEYPRESSTAG_CNT - 1 };
			static _int iSelectKeyboardEvent{ KEYBOARDTAG_CNT - 1 };
			static _int iSelectMouseEvent{ MOUSETAG_CNT - 1 };
			static _int iSelectAnim{ 0 };

			static _string Idx = "%3d";
			static _string Input = "##Input";
			static _string Mouse = "##Mouse";
			static _string Key = "##Key";
			static _string StartT = "##StartT";
			static _string EndT = "##EndT";
			static _string NextAnim = "##NextAnim";
			static _string SupV = "##SupV";

			_int iIndex{ 0 };
			_float TimeAcc = static_cast<_float>(_spAnim->GetTimeAcc());
			for (auto& iter : _AnimEvent)
			{
				_string Index = _string::to_string(iIndex++);

				 ANIMEVENTSECTIONDESC* SectionDesc = static_cast<ANIMEVENTSECTIONDESC*>(iter->OutAnimEventDesc());
				 ANIMCHANGEDESC* ChangeDesc = static_cast<ANIMCHANGEDESC*>(iter->OutOtherEventDesc());

				ImGui::TableNextColumn();
				ImGui::Text(Idx, iIndex);
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (true == ImGui::Combo(Input + Index, &iSelectKeyPressEvent, KEYPRESSTAG, KEYPRESSTAG_CNT))
				{
					SectionDesc->KeyPressType = static_cast<KEYPRESSTYPE>(iSelectKeyPressEvent);
				}
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (true == ImGui::Combo(Key + Index, &iSelectKeyboardEvent, KEYTAG, KEYBOARDTAG_CNT))
				{
					SectionDesc->ubInputKey = static_cast<_ubyte>(iSelectKeyboardEvent);
				}
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (true == ImGui::Combo(Mouse + Index, &iSelectMouseEvent, MOUSETAG, MOUSETAG_CNT))
				{
					SectionDesc->MkEventType = static_cast<MKEVENTTYPE>(iSelectMouseEvent);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float StartTime = static_cast<_float>(SectionDesc->dStartTime);
					ImGui::SliderFloat(StartT + Index, &StartTime, 0.0f, TimeAcc);
					SectionDesc->dStartTime = static_cast<_double>(StartTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					_float EndTime = static_cast<_float>(SectionDesc->dEndTime);
					ImGui::SliderFloat(EndT + Index, &EndTime, 0.0f, TimeAcc);
					SectionDesc->dEndTime = static_cast<_double>(EndTime);
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::Text("Dfadf");
				}
				{
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::DragFloat(SupV + Index, &ChangeDesc->fSupplyAnimValue, 0.01f, 0.0f, 10.f);
				}
				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}