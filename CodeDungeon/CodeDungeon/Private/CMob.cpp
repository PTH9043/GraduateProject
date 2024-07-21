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
#include "UProcessedData.h"
#include "UAnimation.h"

CMob::CMob(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UCharacter(_spDevice, _wstrLayer, _eCloneType),
	m_fDistancefromNearestPlayer{ 0 },
	m_bFoundTarget{ false },
	m_spTargetPlayer{ nullptr },
	m_f3TargetPos{},
	m_dtimeAccumulator{ 0 },
	m_delapsedTime{ 0 },
	m_fActivationRange{ 0 },
	m_fDeactivationRange{0},
	m_isNeedServerSendData{ false },
	m_isNetworkMoveSituation{ false },
	m_isNetworkRotateSituation{ false },
	m_vMovePos{},
	m_fRotateSpeed{DirectX::XMConvertToRadians(90.f)},
	m_isMoveStateActive{ false }
{
}

CMob::CMob(const CMob& _rhs)
	: UCharacter(_rhs), 
	m_fDistancefromNearestPlayer{ 0 },
	m_bFoundTarget{ false },
	m_spTargetPlayer{ nullptr },
	m_f3TargetPos{},
	m_dtimeAccumulator{ 0 },
	m_delapsedTime{ 0 },
	m_fActivationRange{ 0 },
	m_fDeactivationRange{ 0 },
	m_isNeedServerSendData{ false },
	m_isNetworkMoveSituation{ false },
	m_isNetworkRotateSituation{ false },
	m_vMovePos{},
	m_fRotateSpeed{ DirectX::XMConvertToRadians(90.f) },
	m_isMoveStateActive{ false }
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
	MOBSERVERDATA MobServerData = UMethod::ConvertTemplate_Index<MOBSERVERDATA>(_Datas, MOBORDER);
	GetTransform()->SetNewWorldMtx(MobServerData.mWorldMatrix);
	SetNetworkID(MobServerData.iMobID);
	GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
	GetAnimModel()->SetAnimation(MobServerData.iStartAnimIndex);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	if (33 == spGameInstance->GetNetworkOwnerID())
	{
		SetMoveStateActive(true);
		GetAnimModel()->NotApplyAnimPositionDisable();
	}
#else
	GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
#endif
	return S_OK;
}

void CMob::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
}

void CMob::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
	if (true == IsMoveStateActive())
	{
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

	}
	__super::LateTickActive(_dTimeDelta);
}

void CMob::SendPacketTickActive(const _double& _dTimeDelta)
{
	if (true == IsMoveStateActive())
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
#ifdef _ENABLE_PROTOBUFF
		CHARSTATE MonsterStateData;
		VECTOR3 SendPos, SendRotate;
		_double dTimeAcc = GetAnimModel()->GetCurrentAnimation()->GetTimeAcc();
		_int AnimIndex = GetAnimModel()->GetCurrentAnimIndex();
		{
			_float3 vPos = GetTransform()->GetPos(), vRotate = GetTransform()->GetRotationValue();
			PROTOFUNC::MakeVector3(&SendPos, vPos.x, vPos.y, vPos.z);
			PROTOFUNC::MakeVector3(&SendRotate, vRotate.x, vRotate.y, vRotate.z);
		}
		PROTOFUNC::MakeCharState(&MonsterStateData,
			GetNetworkID(), SendPos, SendRotate, AnimIndex, GetAnimationController()->GetAnimState(), IsDamaged());
		spGameInstance->SendProcessPacket(UProcessedData(MonsterStateData, TAG_CS_MONSTERSTATE));
#endif
	}
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

void CMob::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
#ifdef _ENABLE_PROTOBUFF
	__super::ReceiveNetworkProcessData(_ProcessData);
	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_DEAD:
	{
		SC_DEAD scDamaged;
		SetDeathState(true);
	}
	break;
	}
#endif
}

void CMob::FindPlayer()
{
	if (false == GetFoundTargetState())
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		m_spTargetPlayer = spGameInstance->FindPlayerToDistance(GetTransform()->GetPos());
	}
	if (m_spTargetPlayer)
	{
		_float3 CurrentMobPos = GetTransform()->GetPos();
		_float3 CurrentPlayerPos = m_spTargetPlayer->GetTransform()->GetPos();

		CalculateDistanceBetweenPlayers(CurrentPlayerPos, CurrentMobPos);
		SearchForPlayers();
	}
}

void CMob::SearchForPlayers()
{
	if (m_fDistancefromNearestPlayer < m_fActivationRange)
		m_bFoundTarget = true;
	else if(m_fDistancefromNearestPlayer >= m_fDeactivationRange)
		m_bFoundTarget = false;
}

void CMob::CalculateDistanceBetweenPlayers(const _float3& _CurrentPlayerPos, const _float3& _CurrentMobPos)
{
	m_fDistancefromNearestPlayer = _float3::Distance(_CurrentMobPos, _CurrentPlayerPos);
}
#ifdef _ENABLE_PROTOBUFF
void CMob::SendMobStateData()
{

}
void CMob::SendCollisionDamagedData(UPawn* _pPawn)
{
	RETURN_CHECK(false == IsDamaged(), ;);

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	COLLISIONDATA CollisionData;
	PROTOFUNC::MakeCollisionData(&CollisionData, GetNetworkID(), _pPawn->GetNetworkID());
	spGameInstance->SendProcessPacket(UProcessedData(CollisionData, TAG_CS_MONSTERCOLIISION));
	SetDamaged(true);
}
#endif
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

    // ��ǥ ��ġ�� �����ߴ��� Ȯ��
    if (distance < 1.0f)
    {
		currentPosition = targetPosition;
        currentPathIndex++;
    }
	m_f3TargetPos = targetPosition;
}

