#include "ClientDefines.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "URegion.h"
#include "UNavigation.h"
#include "CMummyAnimController.h"
#include "UCell.h"
#include "UMethod.h"
#include "UPlayer.h"
#include "UCollider.h"

CMummy::CMummy(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_MummyType{}
{
}

CMummy::CMummy(const CMummy& _rhs)
	: CMob(_rhs), m_MummyType{}
{
}

void CMummy::Free()
{
}

HRESULT CMummy::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMummy::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);
	return S_OK;
}

void CMummy::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	GetAnimationController()->Tick(_dTimeDelta);
	_int CurAnimState = GetAnimationController()->GetAnimState();
	_float3 CurrentMobPos = GetTransform()->GetPos();
	_float3 CurrentPlayerPos = GetTargetPlayer()->GetTransform()->GetPos();
	SHPTR<UCell> CurrentMobCell = GetCurrentNavi()->GetCurCell();
	SHPTR<UCell> CurrentPlayerCell = GetTargetPlayer()->GetCurrentNavi()->GetCurCell();

	static _double timeAccumulator = 0.0;
	static UNavigation::PathFindingState pathFindingState;
	static bool isPathFinding = false;
	static VECTOR<_float3> path;
	static size_t currentPathIndex = 0;

	if(CurAnimState == UAnimationController::ANIM_MOVE)
	{
		timeAccumulator += _dTimeDelta;
		if (GetFoundTargetState() && timeAccumulator >= 2.0)
		{
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			pathFindingState = spNavigation->StartPathFinding(CurrentMobPos, CurrentPlayerPos, CurrentMobCell, CurrentPlayerCell);
			isPathFinding = true;
			timeAccumulator = 0.0;
		}
		if (isPathFinding)
		{
			SHPTR<UNavigation> spNavigation = GetCurrentNavi();
			if (spNavigation->StepPathFinding(pathFindingState))
			{
				isPathFinding = false;
				if (pathFindingState.pathFound)
				{
					path = spNavigation->OptimizePath(pathFindingState.path, CurrentMobPos, CurrentPlayerPos);
					currentPathIndex = 0; // 경로를 재설정하면 인덱스를 초기화합니다
				}
			}
		}
		if (GetFoundTargetState() && !path.empty())
		{
			MoveAlongPath(path, currentPathIndex, _dTimeDelta);
			_float3 direction = CurrentMobPos - GetTargetPos();
			GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
		}
	}
	else
	{
		_float3 direction = CurrentMobPos - CurrentPlayerPos;
		GetTransform()->SetDirectionFixedUp(direction, _dTimeDelta, 5);
	}


	if(CurAnimState == UAnimationController::ANIM_IDLE)
	{
		GetAnimModel()->TickAnimation(_dTimeDelta);
	}
	else
		GetAnimModel()->TickAnimChangeTransform(GetTransform(), _dTimeDelta);

	_float newHeight = GetCurrentNavi()->ComputeHeight(GetTransform()->GetPos());
	GetTransform()->SetPos(_float3(GetTransform()->GetPos().x, newHeight, GetTransform()->GetPos().z));

	UpdateCollision();
}

void CMummy::LateTickActive(const _double& _dTimeDelta)
{
	__super::LateTickActive(_dTimeDelta);
}

HRESULT CMummy::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CMummy::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void CMummy::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	PAWNTYPE ePawnType = _pEnemy->GetPawnType();
	if (PAWNTYPE::PAWN_CHAR == ePawnType)
	{
		UCharacter* pCharacter = static_cast<UCharacter*>(_pEnemy.get());

		for (auto& iter : pCharacter->GetColliderContainer())
		{
			for (auto& Monster : GetColliderContainer())
			{
				if (true == Monster.second->IsCollision(iter.second))
				{
					GetTransform()->SetPos(GetPrevPos());
				}
			}
		}

		for(auto& iter : GetColliderContainer())
		{
			if (pCharacter->GetAnimModel()->IsCollisionAttackCollider(iter.second))
			{

			}
		}
	}
}


