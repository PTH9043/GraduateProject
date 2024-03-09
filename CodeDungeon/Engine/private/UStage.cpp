#include "EngineDefine.h"
#include "UStage.h"
#include "UGameInstance.h"
#include "UCell.h"
#include "URegion.h"
#include "UTransform.h"
#include "UDefaultCube.h"

UStage::UStage(CSHPTRREF<UDevice> _spDevice)
	: UComponent(_spDevice),
	m_spRegionList{ nullptr },
	m_bInitRegion{ false  },
	m_pDeleteRegion{ nullptr }
{
}

UStage::UStage(const UStage& _rhs)
	: UComponent(_rhs),
	m_spRegionList{ nullptr },
	m_bInitRegion{ false },
	m_pDeleteRegion{ nullptr }
{
}

void UStage::Free()
{
}

HRESULT UStage::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);

	m_spRegionList = Create<REGIONLIST>();

	return S_OK;
}

HRESULT UStage::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return S_OK;
}

HRESULT UStage::AddRender(const _uint& _iIndex)
{
	if (_iIndex >= (*m_spRegionList.get()).size())
		return E_FAIL;

	(*m_spRegionList.get())[_iIndex]->AddRegionRenderGroup();
	return S_OK;
}

void UStage::AddRenderAll()
{
	for (auto& iter : (*m_spRegionList.get()))
		iter.second->AddRegionRenderGroup();
}

SHPTR<URegion> UStage::GetRegion(const _uint& _iIndex)
{
	
	RETURN_CHECK(_iIndex >= (*m_spRegionList.get()).size(), nullptr)
	return (*m_spRegionList.get())[_iIndex];
}

HRESULT UStage::AddCell(const _uint& _iCellIndex, SHPTR<UCell>& _pCell)
{
	RETURN_CHECK_FAILED(_iCellIndex >= (*m_spRegionList.get()).size(), E_FAIL)

	(*m_spRegionList.get())[_iCellIndex]->AddCell(_pCell);
	return S_OK;
}


HRESULT UStage::ModifyCells(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= (*m_spRegionList.get()).size(), E_FAIL)

	(*m_spRegionList.get())[_iCellIndex]->ModifyCells();
	return S_OK;
}

HRESULT UStage::ShowCells(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= (*m_spRegionList.get()).size(), E_FAIL)

	(*m_spRegionList.get())[_iCellIndex]->ShowCells();
	return S_OK;
}

HRESULT UStage::ClearCell(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= (*m_spRegionList.get()).size(), E_FAIL)

	(*m_spRegionList.get())[_iCellIndex]->ClearCell();
	return S_OK;
}

_bool UStage::Load(const _wstring& _wstrPath)
{
	SHPTR<URegion> pRegion = CreateConstructorNative<URegion>(GetDevice());
	if (nullptr != pRegion)
	{
		pRegion->Load(_wstrPath);
		pRegion->Set_Index((_uint)(*m_spRegionList.get()).size());
		(*m_spRegionList.get()).emplace(std::pair<_uint, SHPTR<URegion>>((_uint)(*m_spRegionList.get()).size(), pRegion));
	}
	return true;
}

_bool UStage::Save(const _wstring& _wstrPath)
{
	for (auto& iter : (*m_spRegionList.get()))
	{
		iter.second->Save(_wstrPath);
	}
	return true;
}

HRESULT UStage::CreateRegion(const _uint& _iIndex)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (ImGui::TreeNodeEx("Create_Region", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (auto& iter : (*m_spRegionList.get()))
			{
				char pName[MAX_PATH] = { "" };
				sprintf_s(pName, "%d", iter.second->Get_Index());
				ImGui::Text(pName);
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Add_Region"))
		{
			for (auto& iter : (*m_spRegionList.get()))
			{
				if (iter.first == _iIndex)
				{
					ImGui::TreePop();
					return E_FAIL;
				}

				if (iter.second->Get_Index() == _iIndex)
				{
					ImGui::TreePop();
					return E_FAIL;
				}
			}			
			SHPTR<URegion> pRegion = CreateConstructorNative<URegion>(spGameInstance->GetDevice());
			pRegion->Set_Index(_iIndex);
			(*m_spRegionList.get()).emplace(std::pair<_uint, SHPTR<URegion>>((_uint)(*m_spRegionList.get()).size(), pRegion));

			AddArroundRegion();
		}
		ImGui::TreePop();
	}
	return S_OK;
}

_int UStage::SelectRegion()
{
	_int iIndex = INVALID_MINUS_STAGEVALUE;
	if (ImGui::TreeNodeEx("Select_Region", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (REGIONLIST::iterator it = (*m_spRegionList.get()).begin(); it != (*m_spRegionList.get()).end(); ++it)
		{
			char pName[MAX_PATH] = { "" };
			sprintf_s(pName, "%d", (*it).second->Get_Index());
			if (ImGui::Selectable(pName))
			{
				iIndex = it->first;
			}
		}
		ImGui::TreePop();
	}

	return _int();
}

void UStage::Control_Collider(const _uint& _iIndex)
{
	if (_iIndex >= (*m_spRegionList.get()).size())
		return;

	(*m_spRegionList.get())[_iIndex]->Control_Collider();
	return;
}

HRESULT UStage::Delete_Region(_uint& _iIndex)
{
	if (ImGui::TreeNodeEx("Delete_Region"))
	{
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (REGIONLIST::iterator it = (*m_spRegionList.get()).begin(); it != (*m_spRegionList.get()).end(); ++it)
			{
				char pName[MAX_PATH] = { "" };
				sprintf_s(pName, "%d", (*it).second->Get_Index());
				if (ImGui::Selectable(pName))
				{
					m_pDeleteRegion = (*it).second;
				}
			}
			ImGui::EndListBox();
		}

		if (ImGui::Button("Delete"))
		{
			for (REGIONLIST::iterator it = (*m_spRegionList.get()).begin(); it != (*m_spRegionList.get()).end(); ++it)
			{
				if (m_pDeleteRegion == (*it).second)
				{
					((*it).second).reset();
					(*m_spRegionList.get()).erase(it);
					m_pDeleteRegion.reset();
					_iIndex = 100000;
					break;
				}
			}
		}
		ImGui::TreePop();
	}
	return S_OK;
}

_bool UStage::Is_Collision(SHPTR<UCollider>& _pCollider, SHPTR<URegion>* _ppOut)
{
	for (auto& iter : (*m_spRegionList.get()))
	{
		if (true == iter.second->Is_Collision(_pCollider))
		{
			*_ppOut = iter.second;
			return true;
		}
	}
	return false;
}

_bool UStage::Is_Picking(const _uint& _iIndex)
{
	return _bool();
}

_bool UStage::Is_ModelPicking(const _uint& _iIndex)
{
	return _bool();
}

void UStage::UpdateRegion()
{
	if (false == m_bInitRegion)
	{
		AddArroundRegion();
		m_bInitRegion = true;
	}
}

void UStage::AddArroundRegion()
{
	for (auto& iter : (*m_spRegionList.get()))
	{
		iter.second->ClearNeightborRegion();
		for (auto& value : (*m_spRegionList.get()))
		{
			if (value == iter)
				continue;
			iter.second->Add_NeighborRegion(value.second);
		}
	}
}


