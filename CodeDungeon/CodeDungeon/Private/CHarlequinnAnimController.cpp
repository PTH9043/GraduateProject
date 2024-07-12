#include "ClientDefines.h"
#include "CHarlequinnAnimController.h"
#include "CHarlequinn.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UPlayer.h"
#include "CWarriorPlayer.h"
#include "CShurikenThrowing.h"

CHarlequinnAnimController::CHarlequinnAnimController(CSHPTRREF<UDevice> _spDevice)
    : UAnimationController(_spDevice),
    m_bAttackMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0},
    m_didleRandomValueChoosingTimer{ 0 },
    m_bAttack1FirstTime{false},
    m_iRandomValue{0},
    m_bTauntMode{false},
    m_f3ThrowingPos{},
    m_arrThrowingDir{}
{
}

CHarlequinnAnimController::CHarlequinnAnimController(const CHarlequinnAnimController& _rhs)
    : UAnimationController(_rhs),
    m_bAttackMode{ false },
    m_dlastHitTime{ 0 },
    m_dlastAttackTime{ 0 },
    m_bstartlastHitTime{ false },
    m_blastAttackWasFirst{ false },
    m_dIdleTimer{0},
    m_didleRandomValueChoosingTimer{ 0 },
    m_bAttack1FirstTime{ false },
    m_iRandomValue{ 0 },
    m_bTauntMode{ false },
    m_f3ThrowingPos{},
    m_arrThrowingDir{}
{
}

void CHarlequinnAnimController::Free()
{
}

HRESULT CHarlequinnAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CHarlequinnAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpHarlequinnMob = std::dynamic_pointer_cast<CHarlequinn>(GetOwnerCharacter());

    return S_OK;
}

void CHarlequinnAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    ClearTrigger();
    SetAnimState(-1);

    SHPTR<CHarlequinn> spHarlequinn = m_wpHarlequinnMob.lock();
    SHPTR<UAnimModel> spAnimModel = spHarlequinn->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();
    const _wstring& PlayerAnimName = spHarlequinn->GetTargetPlayer()->GetAnimModel()->GetCurrentAnimation()->GetAnimName();
    _float playerKickedTime = static_pointer_cast<CWarriorPlayer>(spHarlequinn->GetTargetPlayer())->GetWarriorKickedTimeElapsed();
    _float DistanceFromPlayer = spHarlequinn->GetDistanceFromPlayer();
  
    _float3 directionToPlayer = spHarlequinn->GetTransform()->GetPos() - spHarlequinn->GetTargetPlayer()->GetTransform()->GetPos();
    
    _bool FoundPlayer = spHarlequinn->GetFoundTargetState();
    _bool Hit = false;

    if (spHarlequinn->GetPrevHealth() > spHarlequinn->GetHealth())
        Hit = true;

    _float AttackRange = 10.0f;
    // Handle found player state
    if (!FoundPlayer)
    {
        // Handle idle mode with 1/3 probability and 3-second duration
        m_bAttackMode = false;
        m_bTauntMode = false;
        //Idle Timer
        if (CurAnimName == L"Idle")
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
                m_iRandomValue = std::rand() % 4;
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
    else if (FoundPlayer)
    {
        m_bTauntMode = true;
        m_dIdleTimer = 0;
    }

     if (m_bTauntMode)
     {
         UpdateState(spAnimModel, ANIM_TAUNT, L"TAUNT");
         if (CurAnimName == L"Idle break")
         {
             m_bAttackMode = true;
             m_bTauntMode = false;
         }
     }

     // Handle hit state
     if (Hit)
     {
         UpdateState(spAnimModel, ANIM_HIT, L"HIT");
         spAnimModel->SetAnimation(L"Get Hit");
         spHarlequinn->SetPrevHealth(spHarlequinn->GetHealth());
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

         UpdateState(spAnimModel, ANIM_ATTACK, L"ATTACK1");
       /*  if (DistanceFromPlayer > AttackRange)
         {
             UpdateState(spAnimModel, ANIM_MOVE, L"WALK");
         }
         else
         {
             UpdateState(spAnimModel, m_blastAttackWasFirst ? ANIM_ATTACK : ANIM_ATTACK, m_blastAttackWasFirst ? L"ATTACK2" : L"ATTACK1");
         }*/
     }

     if (CurAnimName == L"Attack 1")
     {
         m_f3ThrowingPos = spHarlequinn->GetTransform()->GetPos();
         m_f3ThrowingPos.y += 7.f;

         spHarlequinn->GetTransform()->SetDirectionFixedUp(directionToPlayer, _dTimeDelta, 5);

         if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.2 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.22)
         {
             if ((*spHarlequinn->GetShurikens())[0]->GetTraveledDistance() == 0)
             {
                 m_arrThrowingDir[0] = -spHarlequinn->GetTransform()->GetLook();
                 (*spHarlequinn->GetShurikens())[0]->GetTransform()->SetPos(m_f3ThrowingPos);
                 (*spHarlequinn->GetShurikens())[0]->GetTransform()->SetDirection(-spHarlequinn->GetTransform()->GetRight());            
                 (*spHarlequinn->GetShurikens())[0]->SetThrow(true);
             }          
         }
         else if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.6 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.62)
         {
             if ((*spHarlequinn->GetShurikens())[1]->GetTraveledDistance() == 0)
             {
                 m_arrThrowingDir[1] = -spHarlequinn->GetTransform()->GetLook();
                 (*spHarlequinn->GetShurikens())[1]->GetTransform()->SetPos(m_f3ThrowingPos);
                 (*spHarlequinn->GetShurikens())[1]->GetTransform()->SetDirection(-spHarlequinn->GetTransform()->GetRight());
                 (*spHarlequinn->GetShurikens())[1]->SetThrow(true);
             }
         }
     }

     if ((*spHarlequinn->GetShurikens())[0]->GetTraveledDistance() > 100)
     {
         (*spHarlequinn->GetShurikens())[0]->SetThrow(false);
         (*spHarlequinn->GetShurikens())[0]->SetTraveledDistance(0);
     }
     else
         spHarlequinn->ThrowShurikens(0, _dTimeDelta, m_arrThrowingDir[0]);

     if ((*spHarlequinn->GetShurikens())[1]->GetTraveledDistance() > 100)
     {
         (*spHarlequinn->GetShurikens())[1]->SetThrow(false);
         (*spHarlequinn->GetShurikens())[1]->SetTraveledDistance(0);
     }
     else 
         spHarlequinn->ThrowShurikens(1, _dTimeDelta, m_arrThrowingDir[1]);





     // Check for death
     if (spHarlequinn->GetHealth() <= 0)
     {
         spHarlequinn->SetDeathState(true);
     }

     // Handle death state
     if (spHarlequinn->GetDeathState())
     {
         UpdateState(spAnimModel, ANIM_DEATH, L"DEATH");
     }



    // Tick event
    spAnimModel->TickEvent(spHarlequinn.get(), GetTrigger(), _dTimeDelta);
}
