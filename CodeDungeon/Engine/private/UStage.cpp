#include "EngineDefine.h"
#include "UStage.h"
#include "UGameInstance.h"
#include "UCell.h"
#include "URegion.h"
#include "UTransform.h"
#include "UDefaultCube.h"

UStage::UStage(CSHPTRREF<UDevice> _spDevice)
	: UComponent(_spDevice),
	m_RegionList{ },
	m_bInitRegion{ false  },
	m_pDeleteRegion{ nullptr }
{
}

UStage::UStage(const UStage& _rhs)
	: UComponent(_rhs),
	m_RegionList{ },
	m_bInitRegion{ false },
	m_pDeleteRegion{ nullptr }
{
}

void UStage::Free()
{
}

HRESULT UStage::NativeConstruct()
{
	return S_OK;
}

HRESULT UStage::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	return S_OK;
}

SHPTR<URegion> UStage::GetRegion(const _uint& _iIndex)
{
	RETURN_CHECK(_iIndex >= m_RegionList.size(), nullptr)
	return m_RegionList[_iIndex];
}

HRESULT UStage::AddCell(const _uint& _iCellIndex, SHPTR<UCell>& _pCell)
{
	RETURN_CHECK_FAILED(_iCellIndex >= m_RegionList.size(), E_FAIL)

	m_RegionList[_iCellIndex]->AddCell(_pCell);
	return S_OK;
}


HRESULT UStage::ModifyCells(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= m_RegionList.size(), E_FAIL)

	m_RegionList[_iCellIndex]->ModifyCells();
	return S_OK;
}

HRESULT UStage::ShowCells(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= m_RegionList.size(), E_FAIL)

	m_RegionList[_iCellIndex]->ShowCells();
	return S_OK;
}

HRESULT UStage::ClearCell(const _uint& _iCellIndex)
{
	RETURN_CHECK_FAILED(_iCellIndex >= m_RegionList.size(), E_FAIL)

	m_RegionList[_iCellIndex]->ClearCell();
	return S_OK;
}

_bool UStage::Load(const _wstring& _wstrPath)
{
	SHPTR<URegion> pRegion = CreateConstructorNative<URegion>(GetDevice());
	if (nullptr != pRegion)
	{
		pRegion->Load(_wstrPath);
		pRegion->Set_Index((_uint)m_RegionList.size());
		m_RegionList.emplace(std::pair<_uint, SHPTR<URegion>>((_uint)m_RegionList.size(), pRegion));
	}
	return true;
}

_bool UStage::Save(const _wstring& _wstrPath)
{
	for (auto& iter : m_RegionList)
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
			/*for (auto& iter : m_RegionList)
			{
				char pName[MAX_PATH] = { "" };
				sprintf_s(pName, "%d", iter.second->Get_Index());
				ImGui::Text(pName);
			}*/
			ImGui::EndListBox();
		}

		if (ImGui::Button("Add_Region"))
		{
			/*for (auto& iter : m_RegionList)
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
			}*/
			URegion::REGIONDESC RegionDesc;
			RegionDesc.m_iIndex = _iIndex;			

			SHPTR<URegion> pRegion = std::static_pointer_cast<URegion>(spGameInstance->CloneComp(PROTO_COMP_REGION, {&RegionDesc}));
			m_RegionList.emplace(std::pair<_uint, SHPTR<URegion>>(_iIndex, pRegion));

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
		for (REGIONLIST::iterator it = m_RegionList.begin(); it != m_RegionList.end(); ++it)
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
	if (_iIndex >= m_RegionList.size())
		return;

	m_RegionList[_iIndex]->Control_Collider();
	return;
}

HRESULT UStage::Delete_Region(_uint& _iIndex)
{
	if (ImGui::TreeNodeEx("Delete_Region"))
	{
		if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 3 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (REGIONLIST::iterator it = m_RegionList.begin(); it != m_RegionList.end(); ++it)
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
			for (REGIONLIST::iterator it = m_RegionList.begin(); it != m_RegionList.end(); ++it)
			{
				if (m_pDeleteRegion == (*it).second)
				{
					((*it).second).reset();
					m_RegionList.erase(it);
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
	for (auto& iter : m_RegionList)
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
	for (auto& iter : m_RegionList)
	{
		iter.second->ClearNeightborRegion();
		for (auto& value : m_RegionList)
		{
			if (value == iter)
				continue;
			iter.second->Add_NeighborRegion(value.second);
		}
	}
}


