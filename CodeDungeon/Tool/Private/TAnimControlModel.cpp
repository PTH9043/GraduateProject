#include "ToolDefines.h"
#include "TAnimControlModel.h"
#include "UAnimModel.h"
#include "UTransform.h"
#include "UShader.h"
#include "UMethod.h"
#include "UAnimation.h"

TAnimControlModel::TAnimControlModel(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer,
	const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType), m_spModel{nullptr}, 
	m_ModifyAnimFastSestion{},m_ModifyAnimClipSection{}, m_FastSectionSortSpecs{nullptr}
{
}

TAnimControlModel::TAnimControlModel(const TAnimControlModel& _rhs) : UPawn(_rhs),
m_spModel{ nullptr }, m_ModifyAnimFastSestion{}, m_ModifyAnimClipSection{}, m_FastSectionSortSpecs{nullptr}
{
}

void TAnimControlModel::Free()
{
}

HRESULT TAnimControlModel::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TAnimControlModel::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	AddShader(PROTO_RES_ANIMMODELSHADER, RES_SHADER);
	GetTransform()->SetScale({ 0.05f, 0.05f, 0.05f });

	return S_OK;
}

void TAnimControlModel::SetShowModel(CSHPTRREF<UAnimModel> _spModel)
{
	RETURN_CHECK(nullptr == _spModel, ;);
	m_spModel = _spModel;
	m_AnimationClips.clear();
	for (auto& iter : m_spModel->GetAnimStrings())
	{
		m_AnimationClips.insert(MakePair(UMethod::ConvertWToS(iter.first), iter.second));
	}
}

void TAnimControlModel::AnimControlView()
{
	SelectAnimation();
	ModifyAnimation();
}

void TAnimControlModel::SelectAnimation()
{
	ImGui::Text("SelectAnimation");
	if (ImGui::BeginListBox("Select AnimationControl", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (auto& AnimClip : m_AnimationClips)
		{
			_bool isTrue{ false };
			if (ImGui::Selectable(AnimClip.first.c_str(), &isTrue))
			{
				m_spModel->SetAnimation(AnimClip.second);
			}
		}
		ImGui::EndListBox();
	}
}

void TAnimControlModel::ModifyAnimation()
{
	if (ImGui::TreeNode("ModifyAnimation"))
	{
		if (ImGui::BeginTable("FastSection", 10, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			_float fStartSpot = -1.f;
			_float fEndSpot = -1.f;
			_float fFastValue = 1.f;
			ImGui::TableSetupColumn("StartSpot", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, AnimColumnID_StartSpot);
			ImGui::TableSetupColumn("EndSpot", ImGuiTableColumnFlags_WidthFixed, 0.0f, AnimColumnID_EndSpot);
			ImGui::TableSetupColumn("FastValue", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, AnimColumnID_FastValue);
			ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
			ImGui::TableHeadersRow();
			m_FastSectionSortSpecs = ImGui::TableGetSortSpecs();
			if (m_FastSectionSortSpecs->SpecsDirty)
			{
				// 내가 세운 기준에 의해 정렬한다.
			//	if (items.Size > 1)
			//		qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
				m_FastSectionSortSpecs->SpecsDirty = false;
			}
			ImGui::EndTable();
		}

		ImGui::TreePop();
	}
}

void TAnimControlModel::TickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
		m_spModel->TickAnimation(_dTimeDelta);
}

void TAnimControlModel::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT TAnimControlModel::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (nullptr != m_spModel)
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);

		for (_uint i = 0; i < m_spModel->GetMeshContainerCnt(); ++i)
		{
			// Bind Transform 
			GetTransform()->BindTransformData(GetShader());

			m_spModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
			m_spModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
			// Render
			m_spModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

void TAnimControlModel::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
