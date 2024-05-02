#include "EngineDefine.h"
#include "UCell.h"
#include "UVIBufferCell.h"
#include "URenderer.h"
#include "UGameInstance.h"
#include "UTransform.h"
#ifdef _USE_DEBUGGING
#include "UDefaultCell.h"
#endif


UCell::UCell(CSHPTRREF<UDevice> _spDevice) : UObject(_spDevice),
m_arrPoints{},
m_arrLines{},
m_arrNormals{},
m_arrNeighbors{ -1, -1, -1 },
m_vCenterPos{},
m_iIndex{ 0 },
m_vPlane{},
m_f3Color{},
#ifdef _USE_DEBUGGING
m_spCellVIBuffer{ nullptr },
m_spCellPawn{ nullptr }
#endif
{
}

void UCell::Free()
{
}

HRESULT UCell::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);

	return S_OK;
}

HRESULT UCell::NativeConstruct(ARRAY<_float3, POINT_END>& _Points, const _uint _iIndex)
{
	m_arrPoints[POINT_A] = _Points[POINT_A];
	m_arrPoints[POINT_B] = _Points[POINT_B];
	m_arrPoints[POINT_C] = _Points[POINT_C];

	m_iIndex = _iIndex;
	ResortPoints();

#ifdef _USE_DEBUGGING
	m_spCellVIBuffer = CreateConstructorNative<UVIBufferCell>(GetDevice(), m_arrPoints);
	RETURN_CHECK(nullptr == m_spCellVIBuffer, E_FAIL);
#endif

	_float3 vCenterPos{};
	for (auto& iter : m_arrPoints) {
		m_vCenterPos += iter;
	}
	m_vCenterPos /= 3.f;

#ifdef _USE_DEBUGGING
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spCellPawn = static_pointer_cast<UDefaultCell>(spGameInstance->CloneActorAdd(
		PROTO_ACTOR_DEUBGGINGDEFAULTCELL, { &m_spCellVIBuffer }));
	m_f3Color = _float3(0.6f, 0.f, 0.f);
	m_spCellPawn->SetColor(m_f3Color);
#endif

	return S_OK;
}

HRESULT UCell::NativeConstruct(const CELLDECS& _tCellDesc)
{
	for (_uint i = 0; i < POINT_END; ++i)
	{
		m_arrPoints[i] = _tCellDesc.vPoints[i];
		m_arrNormals[i] = _tCellDesc.vNormal[i];
	}
	for (_uint i = 0; i < LINE_END; ++i)
	{
		m_arrLines[i] = _tCellDesc.vLine[i];
		m_arrNeighbors[i] = _tCellDesc.iNeighbor[i];
	}
	m_iIndex = _tCellDesc.iIndex;

	ResortPoints();
#ifdef _USE_DEBUGGING
	m_spCellVIBuffer = CreateConstructorNative<UVIBufferCell>(GetDevice(), m_arrPoints);
	RETURN_CHECK(nullptr == m_spCellVIBuffer, E_FAIL);
#endif
	_float3 vCenterPos{};
	for (auto& iter : m_arrPoints) {
		m_vCenterPos += iter;
	}
	m_vCenterPos /= 3.f;

#ifdef _USE_DEBUGGING
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spCellPawn = static_pointer_cast<UDefaultCell>(spGameInstance->CloneActorAdd(
		PROTO_ACTOR_DEUBGGINGDEFAULTCELL, { &m_spCellVIBuffer }));
	m_f3Color = _float3(0.6f, 0.f, 0.f);
	m_spCellPawn->SetColor(m_f3Color);
#endif

	return S_OK;
}

#ifdef _USE_DEBUGGING
void UCell::AddCellRenderGroup()
{
	m_spCellPawn->AddRenderer(RENDERID::RI_NONALPHA_LAST);
}
#endif

_bool UCell::IsIn(const _float3& _vPos, _int& _iNeightborIndex, _float3& _vLine)
{
	for (_uint i = 0; i < LINE_END; ++i) {
		_float3 vDir = (_vPos - m_arrPoints[i]);
		vDir.Normalize();

		_float Dot = vDir.Dot(m_arrNormals[i]);
		if (0 < Dot) {
			_iNeightborIndex = m_arrNeighbors[i];
			_vLine = m_arrLines[i];
			return false;
		}
	}
	return true;
}

_bool UCell::IsComparePoints(const _float3& _vPointA, const _float3& _vPointB)
{
	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_A]), _vPointA))
	{
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_B], _vPointB), true);
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_C], _vPointB), true);
	}

	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_B]), _vPointA))
	{
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_C], _vPointB), true);
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_A], _vPointB), true);
	}

	if (DirectX::XMVector3Equal(XMLoadFloat3(&m_arrPoints[POINT_C]), _vPointA))
	{
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_A], _vPointB), true);
		RETURN_CHECK(DirectX::XMVector3Equal(m_arrPoints[POINT_B], _vPointB), true);
	}
	return false;
}

_bool UCell::IsComparePoint(CSHPTRREF<UCell> _pCell)
{
	RETURN_CHECK(nullptr == _pCell, false);
	using namespace DirectX;
	for (_uint i = 0; i < POINT_END; ++i) {
		if (XMVector3Equal(m_arrPoints[i], _pCell->GetPoint(POINT_A)))
		{
			for (_uint j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;

				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_B)), true);
				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_C)), true);
			}
		}
		if (XMVector3Equal(m_arrPoints[i], _pCell->GetPoint(POINT_B)))
		{
			for (_uint j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;

				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_A)), true);
				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_C)), true);
			}
		}
		if (XMVector3Equal(m_arrPoints[i], _pCell->GetPoint(POINT_C)))
		{
			for (_uint j = 0; j < POINT_END; ++j) {
				if (i == j)
					continue;

				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_B)), true);
				RETURN_CHECK(XMVector3Equal(m_arrPoints[j], _pCell->GetPoint(POINT_C)), true);
			}
		}

	}
	return false;
}

const _float UCell::ComputeHeight(const _float3& _vPosition)
{
	return _float3(DirectX::XMPlaneDotCoord(m_vPlane, _vPosition)).y + _vPosition.y;
}

void UCell::ReBuffer()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spCellVIBuffer.reset();
	m_spCellVIBuffer = CreateConstructorNative<UVIBufferCell>(spGameInstance->GetDevice(), m_arrPoints);
}

void UCell::CalculateCrossResult(ARRAY<_float3, POINT_END>& _arrPointsEnd)
{
	_float3 p1{ m_arrPoints[POINT_A].x, 0.f, m_arrPoints[POINT_A].z };
	_float3 p2{ m_arrPoints[POINT_B].x, 0.f, m_arrPoints[POINT_B].z };
	_float3 p3{ m_arrPoints[POINT_C].x, 0.f, m_arrPoints[POINT_C].z };

	_float3 L1 = p2 - p1;
	_float3 L2 = p3 - p2;
	_float3 L3 = p1 - p3;

	_float3 vCross1 = DirectX::XMVector3Cross(L1, L2);
	_float3 vCross2 = DirectX::XMVector3Cross(L2, L3);
	_float3 vCross3 = DirectX::XMVector3Cross(L3, L1);

	_arrPointsEnd[POINT_A] = vCross1;
	_arrPointsEnd[POINT_B] = vCross2;
	_arrPointsEnd[POINT_C] = vCross3;
}

void UCell::ResortPoints()
{
	// Resort Points
	// 같은 시계 방향으로 점들이 배치되도록 하는 것
	bool bIsTrue = true;
	while (bIsTrue) {
		ARRAY<_float3, POINT_END> arrPosList;
		CalculateCrossResult(arrPosList);

		if (arrPosList[0].y < 0)
		{
			std::swap(m_arrPoints[POINT_A], m_arrPoints[POINT_B]);
			continue;
		}
		if (arrPosList[1].y < 0)
		{
			std::swap(m_arrPoints[POINT_B], m_arrPoints[POINT_C]);
			continue;
		}
		if (arrPosList[2].y < 0)
		{
			std::swap(m_arrPoints[POINT_C], m_arrPoints[POINT_A]);
			continue;
		}
		bIsTrue = false;
	}
	MakeLineAndNormal();
}

void UCell::MakeLineAndNormal()
{
	m_arrLines[LINE_AB] = m_arrPoints[POINT_B] - m_arrPoints[POINT_A];
	m_arrLines[LINE_BC] = m_arrPoints[POINT_C] - m_arrPoints[POINT_B];
	m_arrLines[LINE_CA] = m_arrPoints[POINT_A] - m_arrPoints[POINT_C];

	m_arrNormals[LINE_AB] = _float3(m_arrLines[LINE_AB].z * -1.f, 0.f, m_arrLines[LINE_AB].x);
	m_arrNormals[LINE_BC] = _float3(m_arrLines[LINE_BC].z * -1.f, 0.f, m_arrLines[LINE_BC].x);
	m_arrNormals[LINE_CA] = _float3(m_arrLines[LINE_CA].z * -1.f, 0.f, m_arrLines[LINE_CA].x);

	m_vPlane = DirectX::XMPlaneFromPoints(m_arrPoints[POINT_B], m_arrPoints[POINT_A], m_arrPoints[POINT_C]);
}

#ifdef _USE_DEBUGGING

void UCell::ChangeCellColor(const _float3& _vColor)
{
	RETURN_CHECK(nullptr == m_spCellPawn, ;);
	m_f3Color = _vColor;
	m_spCellPawn->SetColor(m_f3Color);
}
void UCell::ReRender()
{
	RETURN_CHECK(nullptr == m_spCellPawn, ;);
	m_spCellPawn->GetVIBuffer().reset();
	m_spCellPawn->SetVIBuffer(m_arrPoints);
}
#endif