#include "ClientDefines.h"
#include "CMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"

CMummyAnimController::CMummyAnimController(CSHPTRREF<UDevice> _spDevice)
    : CMonsterAnimController(_spDevice),
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

CMummyAnimController::CMummyAnimController(const CMummyAnimController& _rhs)
    : CMonsterAnimController(_rhs),
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_patrol(0, 3);
    std::uniform_int_distribution<> dis_attack(0, 1);


    ClearTrigger();
    SetAnimState(-1);
    SHPTR<CMummy> spMummy = m_wpMummyMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMummy->GetAnimModel();

    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _float DistanceFromPlayer = spMummy->GetDistanceFromPlayer();
    _bool FoundPlayer = spMummy->GetFoundTargetState();
    _bool Hit = false;

    if (spMummy->GetPrevHealth() > spMummy->GetHealth())
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
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
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

        if (m_dIdleTimer == 0)
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
                UpdateState(spAnimModel, ANIM_MOVE, L"WALKF");
            }
        }
    }
    else if (FoundPlayer && m_bFoundPlayerFirsttime && !m_bAttackMode)
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
        spMummy->SetPrevHealth(spMummy->GetHealth());
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
    if (spMummy->GetHealth() <= 0)
    {
        spMummy->SetDeathState(true);
    }

    // Handle death state
    if (spMummy->GetDeathState())
    {
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        UpdateState(spAnimModel, ANIM_DEATH, L"DEAD");
    }

    // Tick event
    spAnimModel->TickEvent(spMummy.get(), GetTrigger(), _dTimeDelta);
}