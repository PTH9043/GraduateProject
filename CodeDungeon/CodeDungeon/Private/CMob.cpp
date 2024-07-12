#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UPlayer.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "CMummyAnimController.h"
#include "UCell.h"
#include "UMethod.h"
#include "CMob.h"
#include "UCollider.h"

CMob::CMob(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_fDistancefromNearestPlayer{ 0 },
	m_bFoundTarget{ false },
	m_spTargetPlayer{ nullptr },
	m_f3TargetPos{},
	m_dtimeAccumulator{ 0 },
	m_delapsedTime{ 0 }
{
}

CMob::CMob(const CMob& _rhs)
	: UCharacter(_rhs), 
	m_fDistancefromNearestPlayer{ 0 },
	m_bFoundTarget{ false },
	m_spTargetPlayer{ nullptr },
	m_f3TargetPos{},
	m_dtimeAccumulator{ 0 },
	m_delapsedTime{ 0 }
{
}

void CMob::Free()
{
}

HRESULT CMob::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMob::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);	

#ifdef _ENABLE_PROTOBUFF
	//{
	//	assert(_Datas.size() >= 2);
	//	MOBNETWORKDATA MobNetworkData = UMethod::ConvertTemplate_Index<MOBNETWORKDATA>(_Datas, MOBORDER);
	//	GetTransform()->SetPos(MobNetworkData.vPos);
	//	GetTransform()->RotateFix(MobNetworkData.vRotate);
	//	GetTransform()->SetScale(MobNetworkData.vScale);
	//	GetAnimModel()->SetAnimation(MobNetworkData.iAnimIndex);
	//}

	GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
#else
	GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
#endif
	return S_OK;
}

void CMob::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
#ifndef _ENABLE_PROTOBUFF
	if(m_spTargetPlayer)
	{
		_float3 CurrentMobPos = GetTransform()->GetPos();
		_float3 CurrentPlayerPos = m_spTargetPlayer->GetTransform()->GetPos();

		CalculateDistanceBetweenPlayers(CurrentPlayerPos, CurrentMobPos);
		SearchForPlayers();
	}
#endif
}

void CMob::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	_float3 vPosition{ GetTransform()->GetPos() };
	SHPTR<UCell> newCell{};

	if (false == GetCurrentNavi()->IsMove(vPosition, REF_OUT newCell))
	{
		_float3 closestPoint = GetCurrentNavi()->ClampPositionToCell(vPosition);
		GetTransform()->SetPos(_float3(closestPoint.x, vPosition.y, closestPoint.z));
		vPosition = GetTransform()->GetPos();
	}

	_float newHeight = GetCurrentNavi()->ComputeHeight(vPosition);
	GetTransform()->SetPos(_float3(vPosition.x, newHeight, vPosition.z));

	__super::LateTickActive(_dTimeDelta);
}

HRESULT CMob::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CMob::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}
HRESULT CMob::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CMob::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}


void CMob::SearchForPlayers()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	
	float activationRange = 40.0f;
	float deactivationRange = 80.f;
	
	if (m_fDistancefromNearestPlayer < activationRange)
		m_bFoundTarget = true;
	else if(m_fDistancefromNearestPlayer >= deactivationRange)
		m_bFoundTarget = false;
}

void CMob::CalculateDistanceBetweenPlayers(const _float3& _CurrentPlayerPos, const _float3& _CurrentMobPos)
{
	m_fDistancefromNearestPlayer = _float3::Distance(_CurrentMobPos, _CurrentPlayerPos);
}

void CMob::SetMobPlacement(_int _CellIndex)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SHPTR<UNavigation> spNavigation = GetCurrentNavi();
	SHPTR<UCell> spCell = spNavigation->FindCell(_CellIndex);

	GetTransform()->SetPos(spCell->GetCenterPos());
}

void CMob::MoveAlongPath(const VECTOR<_float3>& path, size_t& currentPathIndex, const _double& _dTimeDelta)
{
    if (currentPathIndex >= path.size())
        return;

    _float3 currentPosition = GetTransform()->GetPos();
	_float3 targetPosition = (path)[currentPathIndex];

    _float3 direction = targetPosition - currentPosition;
    _float distance = direction.Length();
    direction.Normalize();

    // 목표 위치에 도달했는지 확인
    if (distance < 1.0f)
    {
		currentPosition = targetPosition;
        currentPathIndex++;
    }
	m_f3TargetPos = targetPosition;
}

