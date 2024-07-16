#include "ClientDefines.h"
#include "CAnubisAnimController.h"
#include "CAnubis.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"

CAnubisAnimController::CAnubisAnimController(CSHPTRREF<UDevice> _spDevice)
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
    m_dRecvAnimDuration{ 0 },
    m_iRandomValueforAttack{ 0 },
    m_bAttackStart{ false }
{
}

CAnubisAnimController::CAnubisAnimController(const CAnubisAnimController& _rhs)
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
    m_dRecvAnimDuration{ 0 },
    m_iRandomValueforAttack{ 0 },
    m_bAttackStart{ false }
{
}

void CAnubisAnimController::Free()
{
}

HRESULT CAnubisAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CAnubisAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpAnubisMob = std::dynamic_pointer_cast<CAnubis>(GetOwnerCharacter());

    return S_OK;
}

void CAnubisAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_attack(0, 3);


    ClearTrigger();
    SetAnimState(-1);
    SHPTR<CAnubis> spAnubis = m_wpAnubisMob.lock();
    SHPTR<UAnimModel> spAnimModel = spAnubis->GetAnimModel();

    _float AttackRange_Long = 40;
    _float AttackRange_Close = 10.0f;

#ifndef _ENABLE_PROTOBUFF
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _float DistanceFromPlayer = spAnubis->GetDistanceFromPlayer();
    _bool FoundPlayer = spAnubis->GetFoundTargetState();
    _bool Hit = false;

    if (spAnubis->GetPrevHealth() > spAnubis->GetHealth())
        Hit = true;

    _float AttackRange = 10.0f;

    // Handle found player state
    if (FoundPlayer && !m_bFoundPlayerFirsttime)
    {
        UpdateState(spAnimModel, ANIM_AWAKE, L"TOATTACKIDLE");
        m_bFoundPlayerFirsttime = true; 
    }
    else if (!FoundPlayer && m_bFoundPlayerFirsttime)
    {
        m_bAttackMode = false;
        m_bAttackStart = false;

        if (_float3::Distance(spAnubis->GetTransform()->GetPos(), spAnubis->GetOriginPos()) > 2)
        {
            UpdateState(spAnimModel, ANIM_MOVE, L"WALK");
        }
        else
        {
            UpdateState(spAnimModel, ANIM_AWAKE, L"TOGUARDIDLE");
            m_bFoundPlayerFirsttime = false;
        }
    }
    else if (FoundPlayer && m_bFoundPlayerFirsttime && !m_bAttackMode)
    {
        if(CurAnimName == L"Guard Idle to Attack Idle" || CurAnimName == L"Walk")
            m_bAttackMode = true;

    }
    else if (!FoundPlayer && !m_bFoundPlayerFirsttime)
    {
        if(CurAnimName == L"Attack Idle to Guard Idle")
        {
            if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.9)
            {
                UpdateState(spAnimModel, ANIM_IDLE, L"GUARDIDLE");
            }
        }
        else 
            spAnubis->GetTransform()->SetDirectionFixedUp(spAnubis->GetOriginDirection(), _dTimeDelta, 5);
            
    }

    // Handle hit state
    if (Hit)
    {
        spAnimModel->SetAnimation(L"Hit Reaction");
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        spAnubis->SetPrevHealth(spAnubis->GetHealth());
    }

    
    if (m_bAttackMode && !Hit)
    {
        UpdateState(spAnimModel, ANIM_IDLE, L"ATTACKIDLE");

        if(!m_bAttackStart)
        {
            spAnimModel->SetAnimation(L"Attack Idle");
            m_bAttackStart = true;
        }             


        if(m_bAttackStart)
        {
            if (DistanceFromPlayer > AttackRange_Long)
            {
                UpdateState(spAnimModel, ANIM_MOVE, L"WALK");
            }
            else if (DistanceFromPlayer < AttackRange_Long && DistanceFromPlayer > AttackRange_Close)
            {
                UpdateState(spAnimModel, ANIM_ATTACK, L"CAST");
            }
            else if (DistanceFromPlayer < AttackRange_Close)
            {
                if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.9)
                    m_iRandomValueforAttack = dis_attack(gen); 

                if (m_iRandomValueforAttack == 0)
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK1");
                if (m_iRandomValueforAttack == 1)
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK2");
                if (m_iRandomValueforAttack == 2)
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK3");
                if (m_iRandomValueforAttack == 3)
                    UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK4");
            }
        }
    }

    // Check for death
    if (spAnubis->GetHealth() <= 0)
    {
        spAnubis->SetDeathState(true);
    }

    // Handle death state
    if (spAnubis->GetDeathState())
    {
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        UpdateState(spAnimModel, ANIM_DEATH, L"DEATH");
    }

#endif

    // Tick event
    spAnimModel->TickEvent(spAnubis.get(), GetTrigger(), _dTimeDelta);
}

void CAnubisAnimController::ReceiveNetworkProcessData(void* _pData)
{
#ifdef _ENABLE_PROTOBUFF
    SHPTR<CAnubis> spSarcophagus = m_wpAnubisMob.lock();
    SHPTR<UAnimModel> spAnimModel = spSarcophagus->GetAnimModel();
    {
        SC_MONSTERSTATEHAVEPOS* pMonsterData = static_cast<SC_MONSTERSTATEHAVEPOS*>(_pData);
        m_dRecvAnimDuration = pMonsterData->animationtime();

        if (pMonsterData->animationindex() != spAnimModel->GetCurrentAnimIndex())
            spAnimModel->SetAnimation(pMonsterData->animationindex());
    }
#endif
}