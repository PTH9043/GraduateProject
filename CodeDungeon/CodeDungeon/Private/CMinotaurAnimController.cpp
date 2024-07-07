#include "ClientDefines.h"
#include "CMinotaurAnimController.h"
#include "CMinotaur.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"

CMinotaurAnimController::CMinotaurAnimController(CSHPTRREF<UDevice> _spDevice)
    : UAnimationController(_spDevice),
    m_bAttackMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0}
{
}

CMinotaurAnimController::CMinotaurAnimController(const CMinotaurAnimController& _rhs)
    : UAnimationController(_rhs),
    m_bAttackMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0}
{
}

void CMinotaurAnimController::Free()
{
}

HRESULT CMinotaurAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CMinotaurAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpMinotaurMob = std::dynamic_pointer_cast<CMinotaur>(GetOwnerCharacter());

    return S_OK;
}

void CMinotaurAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    ClearTrigger();
    SetAnimState(-1);

    SHPTR<CMinotaur> spMinotaur = m_wpMinotaurMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMinotaur->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _float DistanceFromPlayer = spMinotaur->GetDistanceFromPlayer();
    _bool FoundPlayer = spMinotaur->GetFoundTargetState();
    _bool Hit = spMinotaur->GetHitState();
    _float AttackRange = 10.0f;

    if (!FoundPlayer)
    {
        // Handle idle mode with 1/3 probability and 3-second duration
        m_bAttackMode = false;
        //Idle Timer
        if (CurAnimName == L"idle")
        {
            m_dIdleTimer += _dTimeDelta;
            if (m_dIdleTimer >= 2.0)
            {
                m_dIdleTimer = 0.0;
            }
        }

        if(m_dIdleTimer == 0)
        {
            // Generate a random number between 0 and 3 (inclusive)
            _int randomValue = std::rand() % 4;

            if (randomValue == 0)
            {
                UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
            }
            else if (randomValue != 0)
            {
                UpdateState(spAnimModel, ANIM_MOVE, L"WALK");
            }
        }
    }
    else
    {
        m_bAttackMode = true;
    }

    // Handle hit state
    if (Hit)
    {
        UpdateState(spAnimModel, ANIM_HIT, L"HIT");
        spAnimModel->SetAnimation(L"hit_1");
        m_bstartlastHitTime = true;
        spMinotaur->SetHitstate(false);
    }

    // Handle attack mode state
    if (m_bAttackMode && !Hit)
    {
        m_dlastAttackTime += _dTimeDelta;
        if (m_dlastAttackTime > 3.0)
        {
            m_blastAttackWasFirst = !m_blastAttackWasFirst;
            m_dlastAttackTime = 0;
        }

        if (DistanceFromPlayer > AttackRange)
        {
            UpdateState(spAnimModel, ANIM_MOVE, L"RUN");
        }
        else
        {
            UpdateState(spAnimModel, m_blastAttackWasFirst ? ANIM_ATTACK : ANIM_ATTACK, m_blastAttackWasFirst ? L"ATTACK" : L"KICK_1");
        }
    }

    // Check for death
    if (spMinotaur->GetHealth() <= 0)
    {
        spMinotaur->SetDeathState(true);
    }

    // Handle death state
    if (spMinotaur->GetDeathState())
    {
        UpdateState(spAnimModel, ANIM_DEATH, L"DEATH");
    }

    // Tick event
    spAnimModel->TickEvent(spMinotaur.get(), GetTrigger(), _dTimeDelta);
}
