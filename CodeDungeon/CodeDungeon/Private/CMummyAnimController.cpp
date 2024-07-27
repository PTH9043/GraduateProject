#include "ClientDefines.h"
#include "CMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UPlayer.h"
#include "USound.h"

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
    if (FoundPlayer && !spMummy->GetTargetPlayer()->GetDeathState())
    {
        if(!m_bFoundPlayerFirsttime)
        {
            UpdateState(spAnimModel, ANIM_AWAKE, L"WAKEUP");
            if (spMummy->GetMummyType() == CMummy::TYPE_LYING)
                spAnimModel->SetAnimation(L"openLaying");
            else if (spMummy->GetMummyType() == CMummy::TYPE_STANDING)
                spAnimModel->SetAnimation(L"openStanding");
            m_bFoundPlayerFirsttime = true;
            m_dIdleTimer = 0;
        }
    }
     if ((!FoundPlayer && m_bFoundPlayerFirsttime) || spMummy->GetTargetPlayer()->GetDeathState())
    {
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        // Handle idle mode with 1/3 probability and 3-second duration
        m_bAttackMode = false;
        m_bTauntMode = false;
        //Idle Timer
        if (CurAnimName == L"idle")
        {
            m_dIdleTimer += _dTimeDelta;
            if (m_dIdleTimer >= 1.0)
            {
                m_dIdleTimer = 0.0;
            }

        }
        else
            m_dIdleTimer = 0;

        if (m_dIdleTimer == 0)
        {
            m_didleRandomValueChoosingTimer += _dTimeDelta;
            if (m_didleRandomValueChoosingTimer > 3)
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
     if (FoundPlayer && m_bFoundPlayerFirsttime && !m_bAttackMode)
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

    if (CurAnimName == L"attack1")
    {
        USound* AttackSound1 = spGameInstance->BringSound(L"Attack1_VO_1").get();
        USound* SwooshSound1 = spGameInstance->BringSound(L"ClothWhoosh_1").get();   
        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.8)
        {
            AttackSound1->Stop();
            SwooshSound1->Stop();
        }
        else if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.1 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.11)
        {
            AttackSound1->Play();
            AttackSound1->UpdateSound3D(spMummy->GetTransform(), _float3(0, 0, 0), spMummy->GetTargetPlayer()->GetTransform());
            SwooshSound1->Play();
            SwooshSound1->UpdateSound3D(spMummy->GetTransform(), _float3(0, 0, 0), spMummy->GetTargetPlayer()->GetTransform());
        }
    }

    spGameInstance->HandleSounds3DForAnimation(spMummy, spMummy->GetTargetPlayer(), L"attack1", L"Attack1_VO_1", 0.1, 0.8);
    spGameInstance->HandleSounds3DForAnimation(spMummy, spMummy->GetTargetPlayer(), L"attack1", L"ClothWhoosh_1", 0.1, 0.8);
    spGameInstance->HandleSounds3DForAnimation(spMummy, spMummy->GetTargetPlayer(), L"attack2", L"Attack1_VO_2", 0.1, 0.8);
    spGameInstance->HandleSounds3DForAnimation(spMummy, spMummy->GetTargetPlayer(), L"attack2", L"ClothWhoosh_2", 0.1, 0.8);

    if (CurAnimName == L"attack2")
    {
        USound* AttackSound2 = spGameInstance->BringSound(L"Attack1_VO_2").get();
        USound* SwooshSound2 = spGameInstance->BringSound(L"ClothWhoosh_2").get();
        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.8)
        {
            AttackSound2->Stop();
            SwooshSound2->Stop();
        }
        else if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.1 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.11)
        {
            AttackSound2->Play();
            AttackSound2->UpdateSound3D(spMummy->GetTransform(), _float3(0, 0, 0), spMummy->GetTargetPlayer()->GetTransform());
            SwooshSound2->Play();
            SwooshSound2->UpdateSound3D(spMummy->GetTransform(), _float3(0, 0, 0), spMummy->GetTargetPlayer()->GetTransform());
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