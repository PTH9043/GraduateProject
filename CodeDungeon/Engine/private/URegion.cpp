#include "EngineDefine.h"
#include "URegion.h"
#include "UGameInstance.h"
#include "UCell.h"
#include "UTransform.h"
#include "UDefaultDebugging.h"
#include "UCollider.h"
#include "UNavigation.h"
#include "UMethod.h"

void URegion::tagCubeObjs::Create(SHPTRREF<UCell> _pCell)
{
#ifdef _USE_DEBUGGING
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	UDefaultDebugging::DEBUGTYPE eDebugType{ UDefaultDebugging::DEBUGTYPE::DEBUG_END };
	eDebugType = UDefaultDebugging::DEBUGTYPE::DEBUG_CUBE;
	// Clone Actor

	VOIDDATAS vecDatas;
	vecDatas.push_back(&eDebugType);

	spCube1 = static_pointer_cast<UDefaultDebugging>(spGameInstance->CloneActorAddAndNotInLayer(
		PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT, vecDatas));;
	spCube1->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_A));
	spCube1->SetColor(_float4(1.0f, 1.0f, 1.0f, 0.5f));
	spCube2 = static_pointer_cast<UDefaultDebugging>(spGameInstance->CloneActorAddAndNotInLayer(
		PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT, vecDatas));;
	spCube2->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_B));
	spCube2->SetColor(_float4(1.0f, 1.0f, 1.0f, 0.5f));
	spCube3 = static_pointer_cast<UDefaultDebugging>(spGameInstance->CloneActorAddAndNotInLayer(
		PROTO_ACTOR_DEUBGGINGDEFAULTOBJECT, vecDatas));;
	spCube3->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_C));
	spCube3->SetColor(_float4(1.0f, 1.0f, 1.0f, 0.5f));

	spCell = _pCell;
#endif
}

void URegion::tagCubeObjs::Rebalance()
{
	if (nullptr == spCell)
		return;

	if (nullptr == spCube1 || nullptr == spCube2 || nullptr == spCube3)
		return;

	spCube1->GetTransform()->SetPos(spCell->GetPoint(UCell::POINT_A));
	spCube2->GetTransform()->SetPos(spCell->GetPoint(UCell::POINT_B));
	spCube3->GetTransform()->SetPos(spCell->GetPoint(UCell::POINT_C));

	spCell->ReBuffer();
}

void URegion::tagCubeObjs::AddCubesRenderGroup()
{
	if (nullptr != spCube1)
		spCube1->AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);

	if (nullptr != spCube2)
		spCube2->AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);

	if (nullptr != spCube3)
		spCube3->AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
}

HRESULT URegion::AddRegionRenderGroup()
{
	for (auto& iter : m_CubeObjList)
		iter.AddCubesRenderGroup();

	return S_OK;
}

URegion::URegion(CSHPTRREF<UDevice> _spDevice)
	: UComponent(_spDevice),
	m_spNavigation{ nullptr },
	m_iIndex{ 0 },
	m_NeighborRegion{}
#ifdef _USE_DEBUGGING
	, m_CubeObjList{}
#endif
{
}

URegion::URegion(const URegion& _rhs)
	: UComponent(_rhs),
	m_spNavigation{ nullptr },
	m_iIndex{ 0 },
	m_NeighborRegion{}
#ifdef _USE_DEBUGGING
	, m_CubeObjList{}
#endif
{
}

void URegion::Free()
{
}

HRESULT URegion::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (nullptr == m_spNavigation)
		m_spNavigation = CreateConstructorNative<UNavigation>(spGameInstance->GetDevice());
	return S_OK;
}

HRESULT URegion::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	return S_OK;
}

HRESULT URegion::AddCell(SHPTR<UCell>& _pCell)
{
	if (nullptr != m_spNavigation)
	{
		m_spNavigation->AddCell(_pCell);
#ifdef _USE_DEBUGGING
		CUBOBJS tObjs;
		tObjs.Create(_pCell);
		m_CubeObjList.push_back(tObjs);
#endif
	}
	return S_OK;
}

HRESULT URegion::ModifyCells()
{
	RETURN_CHECK_FAILED(nullptr == m_spNavigation, E_FAIL);

	if (ImGui::TreeNodeEx("Region_Modfiy_Cells", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SHPTR<CELLCONTAINER> pCells = m_spNavigation->GetCellContainer();
		for (CELLCONTAINER::iterator it = pCells->begin(); it != pCells->end(); ++it)
		{
			char TEXT[MAX_PATH] = {};
			sprintf_s(TEXT, "Cell: %d", (*it)->GetIndex());
			if (ImGui::TreeNodeEx(TEXT, ImGuiTreeNodeFlags_DefaultOpen))
			{
#ifdef _USE_DEBUGGING
				_bool bTrue1 = false, bTrue2 = false, bTrue3 = false;
#endif
				_float3 v3Pos1, v3Pos2, v3Pos3;
				{
					v3Pos1 = (*it)->GetPoint(UCell::POINT_A);
					_float PosArr[3] = { v3Pos1.x, v3Pos1.y, v3Pos1.z };
#ifdef _USE_DEBUGGING
					bTrue1 = ImGui::InputFloat3("P1", PosArr, "%.2f");
#else
					ImGui::InputFloat3("P1", PosArr, "%.2f");
#endif
					v3Pos1 = _float3(PosArr[0], PosArr[1], PosArr[2]);
					(*it)->SetPoint(UCell::POINT_A, v3Pos1);
				}
				{
					v3Pos2 = (*it)->GetPoint(UCell::POINT_B);
					_float PosArr[3] = { v3Pos2.x, v3Pos2.y, v3Pos2.z };
#ifdef _USE_DEBUGGING
					bTrue2 = ImGui::InputFloat3("P1", PosArr, "%.2f");
#else
					ImGui::InputFloat3("P1", PosArr, "%.2f");
#endif
					v3Pos2 = _float3(PosArr[0], PosArr[1], PosArr[2]);
					(*it)->SetPoint(UCell::POINT_B, v3Pos2);
				}
				{
					v3Pos3 = (*it)->GetPoint(UCell::POINT_C);
					_float PosArr[3] = { v3Pos3.x, v3Pos3.y, v3Pos3.z };
#ifdef _USE_DEBUGGING
					bTrue3 = ImGui::InputFloat3("P1", PosArr, "%.2f");
#else
					ImGui::InputFloat3("P1", PosArr, "%.2f");
#endif
					v3Pos3 = _float3(PosArr[0], PosArr[1], PosArr[2]);
					(*it)->SetPoint(UCell::POINT_C, v3Pos3);
				}
#ifdef _USE_DEBUGGING
				if (true == bTrue1 || true == bTrue2 || true == bTrue3)
				{
					for (auto& iter : m_CubeObjList)
						iter.Rebalance();
				}
#endif
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	return S_OK;
}

HRESULT URegion::ShowCells()
{
	RETURN_CHECK_FAILED(nullptr == m_spNavigation, E_FAIL)

	if (ImGui::TreeNodeEx("Region_Show_Cells", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SHPTR<CELLCONTAINER> pCells = m_spNavigation->GetCellContainer();
		_uint iIndex = 0;
		for (CELLCONTAINER::iterator it = pCells->begin(); it != pCells->end(); ++it)
		{
			char TEXT[MAX_PATH] = {};
			sprintf_s(TEXT, "Cell: %d", iIndex++);
			if (ImGui::TreeNodeEx(TEXT, ImGuiTreeNodeFlags_DefaultOpen))
			{
				{
					_float3 v3Pos = (*it)->GetPoint(UCell::POINT_A);
					sprintf_s(TEXT, "PA: (%.2f, %.2f, %.2f)", v3Pos.x, v3Pos.y, v3Pos.z);
					ImGui::Text(TEXT);
				}
				{
					_float3 v3Pos = (*it)->GetPoint(UCell::POINT_B);
					sprintf_s(TEXT, "PB: (%.2f, %.2f, %.2f)", v3Pos.x, v3Pos.y, v3Pos.z);
					ImGui::Text(TEXT);
				}
				{
					_float3 v3Pos = (*it)->GetPoint(UCell::POINT_C);
					sprintf_s(TEXT, "PC: (%.2f, %.2f, %.2f)", v3Pos.x, v3Pos.y, v3Pos.z);
					ImGui::Text(TEXT);
				}

				if (ImGui::Button("Delete"))
				{
#ifdef _USE_DEBUGGING
					for (LIST<CUBOBJS>::iterator v = m_CubeObjList.begin(); v != m_CubeObjList.end(); ++v)
					{
						if ((*v).spCell == (*it))
						{
							(*it).reset();
							m_CubeObjList.erase(v);
							break;
						}
					}
#endif
					pCells->erase(it);
					ImGui::TreePop();
					break;
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	return S_OK;
}

HRESULT URegion::ClearCell()
{
	return S_OK;
}

_bool URegion::Load(const _wstring& _wstrPath)
{
	RETURN_CHECK(nullptr == m_spNavigation, false)
	m_spNavigation->Load(_wstrPath.c_str());
#ifdef _USE_DEBUGGING
	for (auto iter : *m_spNavigation->GetCells())
	{
		CUBOBJS tObjs;
		tObjs.Create(iter);
		m_CubeObjList.push_back(tObjs);
	}
#endif

	return S_OK;
}

_bool URegion::Save(const _wstring& _wstrPath)
{
	RETURN_CHECK(nullptr == m_spNavigation, false)
	m_spNavigation->Save(_wstrPath.c_str());
	return S_OK;
}

_bool URegion::Is_Collision(SHPTR<UCollider>& _pCollider)
{
	if (nullptr == _pCollider || nullptr == m_spNavigation)
		return false;

	return m_spNavigation->IsCollision(_pCollider);
}

void URegion::Control_Collider()
{
	if (nullptr == m_spNavigation)
		return;

	if (nullptr != m_spNavigation->GetCollider())
	{
		if (ImGui::TreeNodeEx("ControlCollider", ImGuiTreeNodeFlags_DefaultOpen))
		{
			UCollider::COLLIDERDESC tCollData;
			tCollData.vScale = m_spNavigation->GetCollider()->GetScale();
			tCollData.vTranslation = m_spNavigation->GetCollider()->GetTranslate();

			_float Translate[3] = { tCollData.vTranslation.x,  tCollData.vTranslation.y, tCollData.vTranslation.z };
			_float CollSclae[3] = { tCollData.vScale.x ,  tCollData.vScale.y , tCollData.vScale.z };

			if (ImGui::InputFloat3("FindT", Translate, "%.4f"))
			{
				m_spNavigation->GetCollider()->SetTranslate(_float3(Translate[0], Translate[1], Translate[2]));
			}
			if (ImGui::InputFloat3("TindS", CollSclae, "%.4f"))
			{
				m_spNavigation->GetCollider()->SetScale(_float3(CollSclae[0], CollSclae[1], CollSclae[2]));
			}
			ImGui::TreePop();
		}
	}
}

void URegion::Add_NeighborRegion(SHPTR<URegion>& _pRegion)
{
	if (nullptr == _pRegion || nullptr == m_spNavigation)
		return;

	SHPTR<UCollider> pCollider = _pRegion->GetNavigation()->GetCollider();
	
	if (true == m_spNavigation->IsCollision(pCollider))
	{
		for (auto& iter : m_NeighborRegion)
		{
			if (iter == _pRegion)
				return;
		}
		m_NeighborRegion.push_back(_pRegion);
	}
}
