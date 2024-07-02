#include "ClientDefines.h"
#include "CMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"

CMummyAnimController::CMummyAnimController(CSHPTRREF<UDevice> _spDevice)
	: UAnimationController(_spDevice)
{
}

CMummyAnimController::CMummyAnimController(const CMummyAnimController& _rhs)
	: UAnimationController(_rhs)
{
}

void CMummyAnimController::Free()
{
}

HRESULT CMummyAnimController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CMummyAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

	m_wpMummyMob = std::dynamic_pointer_cast<CMummy>(GetOwnerCharacter());

	return S_OK;
}

void CMummyAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    // Reset Trigger
    ClearTrigger();
    /* Reset */
    SetAnimState(-1);

    static _bool AttackMode = false;
    static _bool TauntMode = false;
    static _bool hitExecuted = false; // Track if hit animation has been played
    static _double lastHitTime = 0;   // Track the time when hit animation was last executed
    static _double lastAttackTime = 0;
    static _bool startlastHitTime = false;
    static _bool lastAttackWasFirst = false;


    SHPTR<CMummy> spMummy = m_wpMummyMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMummy->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _float DistanceFromPlayer = spMummy->GetDistanceFromPlayer();
    _bool FoundPlayer = spMummy->GetFoundTargetState();
    _bool Move = false;

    _float AttackRange = 13.f;
    _bool Hit = spMummy->GetHitState();
    _bool Death = spGameInstance->GetDIKeyDown(DIK_9);

    if (CurAnimName == L"death")
        Death = true;

    if (FoundPlayer && !AttackMode && !TauntMode)
    {
        UpdateState(spAnimModel, ANIM_AWAKE, L"WAKEUP");
    }

    if (CurAnimName == L"openLaying" || CurAnimName == L"openStanding")
        TauntMode = true;

    // Check if hit animation can be executed (2 seconds cooldown)
    if (Hit && !hitExecuted && (lastHitTime == 0.0))
    {
        UpdateState(spAnimModel, ANIM_HIT, L"HIT");
        hitExecuted = true; // Set the flag to indicate hit animation played
        startlastHitTime = true;
    }
    
    if(lastHitTime > 3.0)
    {
        startlastHitTime = false;
        lastHitTime = 0;
    }

    if (startlastHitTime)
        lastHitTime += _dTimeDelta;

    // Reset hitExecuted flag if the current animation is not HIT
    if (spAnimModel->GetCurrentAnimation()->GetAnimName() != L"HIT")
    {
        hitExecuted = false;
    }


    if (TauntMode)
    {
        UpdateState(spAnimModel, ANIM_TAUNT, L"TAUNT");
    }

    if (CurAnimName == L"taunt")
    {
        AttackMode = true;
        TauntMode = false;
    }

    if (AttackMode)
    {
        if (!Hit)
        {
            lastAttackTime += _dTimeDelta;
            if (lastAttackTime > 3.0)
            {
                lastAttackWasFirst = !lastAttackWasFirst;
                lastAttackTime = 0;
            }

            if (DistanceFromPlayer > AttackRange)
                UpdateState(spAnimModel, ANIM_MOVE, L"WALKF");
            else
            {
                if (lastAttackWasFirst)
                {
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK02");
                }
                else
                {
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK01");
                }
            }
        }
    }



    if (Death)
    {
        UpdateState(spAnimModel, ANIM_DEATH, L"DEAD");
    }

    spAnimModel->TickEvent(spMummy.get(), GetTrigger(), _dTimeDelta);
}
