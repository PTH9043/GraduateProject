#include "EngineDefine.h"
#include "URegion.h"
#include "UGameInstance.h"
#include "UCell.h"
#include "UTransform.h"
#include "UDefaultCube.h"

void URegion::tagCubeObjs::Create(SHPTRREF<UCell> _pCell)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<UDefaultCube> spCube = std::static_pointer_cast<UDefaultCube>(spGameInstance->CloneActorAdd(L"Proto_Actor_CubeObject"));

	_uint iCount = 0;
	spCube1 = spCube;
	spCube1->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_A));
	spCube2 = spCube;
	spCube2->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_B));
	spCube3 = spCube;
	spCube3->GetTransform()->SetPos(_pCell->GetPoint(UCell::POINT_C));

	spCell = _pCell;
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

void URegion::tagCubeObjs::Render_Cubes(CSHPTRREF<URenderer> _pRenderer)
{

}

URegion::URegion(CSHPTRREF<UDevice> _spDevice)
	: UResource(_spDevice),
	m_spNavigation{ nullptr },
	m_iIndex{ 0 },
	m_NeighborRegion{}
#ifdef _USE_DEBUGGING
	, m_CubeObjList{}
#endif
{
}

URegion::URegion(const URegion& _rhs)
	: UResource(_rhs),
	m_spNavigation{nullptr},
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
	return S_OK;
}

HRESULT URegion::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (nullptr == m_spNavigation)
		m_spNavigation = UNavigation::Create(m_pDevice, m_pContext);


	return E_NOTIMPL;
}
