#include "ClientDefines.h"
#include "CMimicAnimController.h"
#include "CMimic.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"

CMimicAnimController::CMimicAnimController(CSHPTRREF<UDevice> _spDevice)
    : UAnimationController(_spDevice),
    m_bAttackMode{ false },
    m_bTauntMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0},
    m_bFoundPlayerFirsttime{false},
    m_didleRandomValueChoosingTimer{0},
    m_iRandomValue{ 0 },
    m_dRecvAnimDuration{ 0 }
{
}

CMimicAnimController::CMimicAnimController(const CMimicAnimController& _rhs)
    : UAnimationController(_rhs),
    m_bAttackMode{ false },
    m_bTauntMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0},
    m_bFoundPlayerFirsttime{ false },
    m_didleRandomValueChoosingTimer{ 0 },
    m_iRandomValue{ 0 },
    m_dRecvAnimDuration{ 0 }
{
}

void CMimicAnimController::Free()
{
}

HRESULT CMimicAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CMimicAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpMimicMob = std::dynamic_pointer_cast<CMimic>(GetOwnerCharacter());

    return S_OK;
}

void CMimicAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_patrol(0, 3);
    std::uniform_int_distribution<> dis_attack(0, 1);


    ClearTrigger();
    SetAnimState(-1);
    SHPTR<CMimic> spMimic = m_wpMimicMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMimic->GetAnimModel();

#ifndef _ENABLE_PROTOBUFF
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _float DistanceFromPlayer = spMimic->GetDistanceFromPlayer();
    _bool FoundPlayer = spMimic->GetFoundTargetState();
    _bool Hit = false;

    if (spMimic->GetPrevHealth() > spMimic->GetHealth())
        Hit = true;

    _float AttackRange = 10.0f;

    // Handle found player state
    if (FoundPlayer && !m_bFoundPlayerFirsttime)
    {
        UpdateState(spAnimModel, ANIM_AWAKE, L"WAKEUP");
        m_bFoundPlayerFirsttime = true;
        m_dIdleTimer = 0;
    }
    else if (!FoundPlayer && m_bFoundPlayerFirsttime)
    {
        // Handle idle mode with 1/3 probability and 3-second duration
        m_bAttackMode = false;
        m_bTauntMode = false;
        //Idle Timer
        if (CurAnimName == L"idle")
        {
            m_dIdleTimer += _dTimeDelta;
            if (m_dIdleTimer >= 2.0)
            {
                m_dIdleTimer = 0.0;
            }
          
        }
        else
            m_dIdleTimer = 0;

        if(m_dIdleTimer == 0)
        {
            m_didleRandomValueChoosingTimer += _dTimeDelta;
            if (m_didleRandomValueChoosingTimer > 2)
            {
                m_iRandomValue = dis_patrol(gen);
            }

            if (m_iRandomValue == 0)
            {
                UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
            }
            else if (m_iRandomValue != 0)
            {
                UpdateState(spAnimModel, ANIM_MOVE, L"WALK");
            }
        }
    }
    else if (FoundPlayer && m_bFoundPlayerFirsttime)
    {
        m_bTauntMode = true;
        m_dIdleTimer = 0;
    }

    // Handle taunt mode state
    if (CurAnimName == L"openLaying" || CurAnimName == L"openStanding")
    {
        m_bTauntMode = true;
    }

    if (m_bTauntMode)
    {
        UpdateState(spAnimModel, ANIM_TAUNT, L"TAUNT");
        if (CurAnimName == L"taunt")
        {
            m_bAttackMode = true;
            m_bTauntMode = false;
        }
    }

    // Handle hit state
    if (Hit)
    {   
        spAnimModel->SetAnimation(L"gotHit");
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        spMimic->SetPrevHealth(spMimic->GetHealth());
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
            UpdateState(spAnimModel, ANIM_MOVE, L"WALKF");
        }
        else
        {
            UpdateState(spAnimModel, m_blastAttackWasFirst ? ANIM_ATTACK : ANIM_ATTACK, m_blastAttackWasFirst ? L"ATTACK02" : L"ATTACK01");
        }
    }

    // Check for death
    if (spMimic->GetHealth() <= 0)
    {
        spMimic->SetDeathState(true);
    }

    // Handle death state
    if (spMimic->GetDeathState())
    {
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        UpdateState(spAnimModel, ANIM_DEATH, L"DEAD");
    }

    // Tick event
    spAnimModel->TickEvent(spMimic.get(), GetTrigger(), _dTimeDelta);
#else
spAnimModel->TickAnimChangeTransform(spMimic->GetTransform(), _dTimeDelta);
#endif
}

void CMimicAnimController::ReceiveNetworkProcessData(void* _pData)
{
#ifdef _ENABLE_PROTOBUFF
    SHPTR<CMimic> spSarcophagus = m_wpMimicMob.lock();
    SHPTR<UAnimModel> spAnimModel = spSarcophagus->GetAnimModel();
    {
        CHARSTATE* pMonsterData = static_cast<CHARSTATE*>(_pData);
        if (pMonsterData->animationindex() != spAnimModel->GetCurrentAnimIndex())
        {
            spAnimModel->SetAnimation(pMonsterData->animationindex());
        }
    }
#endif
}