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
	m_isSendDataToBehavior{false},
	m_isFirstFoundState{ false },
	m_iMobType{0},
	m_isMobAlreadyDeadState{ false },
	m_isDeadDissolveEnable{false}
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
	m_isSendDataToBehavior{ false },
	m_isFirstFoundState{ false },
	m_iMobType{0},
	m_isMobAlreadyDeadState{false},
	m_isDeadDissolveEnable{ false }
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
	GetCurrentNavi()->FindCell(GetTransform()->GetPos());
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	spGameInstance->AddCollisionPawnList(ThisShared<UPawn>());
#else
	GetTransform()->SetScale({ 0.7f, 0.7f, 0.7f });
	SetTargetPlayer(nullptr);
	m_isSendDataToBehavior = true;
#endif
	return S_OK;
}

void CMob::TickActive(const _double& _dTimeDelta)
{
	if (true == m_isDeadDissolveEnable)
		SetActive(false);

	CalculateDistanceBetweenPlayers(GetTargetPlayer()->GetTransform()->GetPos(), GetTransform()->GetPos());
	SearchForPlayers();
	__super::TickActive(_dTimeDelta);
}

void CMob::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
}

void CMob::NetworkTickActive(const _double& _dTimeDelta)
{
	SetFoundTargetState(false);
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
	if (true == IsDamaged())
	{
		SetDamaged(false);
	}
}

void CMob::ReceiveNetworkProcessData(const UProcessedData& _ProcessData)
{
#ifdef _ENABLE_PROTOBUFF

	switch (_ProcessData.GetDataType())
	{
	case TAG_SC_MONSTERSTATE:
	{
		static MOBSTATE MobState;
		MobState.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());

		if (false == MobState.triggeron())
		{
			GetAnimationController()->ReceiveNetworkProcessData(&MobState);
			GetTransform()->SetPos(_float3{ MobState.posx(), MobState.posy(), MobState.posz() });
			GetTransform()->RotateFix(_float3{ MobState.rotatex(), MobState.rotatey(), MobState.rotatez() });
	//		SetFoundTargetState(MobState.foundon());
		}
		else
		{
			GetAnimModel()->UpdateCurAnimationToRatio(MobState.animtime());
			SetDeathState(true);
			SetActive(false);
			SetDeadDissolveEnable(true);
			m_isMobAlreadyDeadState = true;
			SetElapsedTime(1000.0);
		}
		MobState.Clear();
	}
	break;
	case TAG_SC_DAMAGED:
	{
		static SC_DAMAGED scDamaged;
		scDamaged.ParseFromArray(_ProcessData.GetData(), _ProcessData.GetDataSize());
		if (0 >= scDamaged.hp())
		{
			SetDeathState(true);
			SetHealth(0);
		}
		else
		{
			SetHealth(scDamaged.hp());
			SetDamaged(true);
		}
		scDamaged.Clear();
	}
	break;
	}
#endif
}

#ifdef _ENABLE_PROTOBUFF
void CMob::SendMobStateData()
{

}

void CMob::SendCollisionData(UPawn* _pPawn, _float _fDamaged)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	CS_DAMAGED csPlayerDamaged;
	_int NetworkID = GetNetworkID();
	{
		PROTOFUNC::MakeCsDamaged(&csPlayerDamaged, NetworkID, _fDamaged);
	}
	spGameInstance->SendProtoData(UProcessedData(NetworkID, csPlayerDamaged,
		TAG_CS_MONSTERCOLIISION));
}

#endif

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

    if (distance < 1.0f)
    {
        currentPathIndex++;
    }
	m_f3TargetPos = targetPosition;
}

