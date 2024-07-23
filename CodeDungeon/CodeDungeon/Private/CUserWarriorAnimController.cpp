#include "ClientDefines.h"
#include "CUserWarriorAnimController.h"
#include "CWarriorPlayer.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UProcessedData.h"
#include "UMethod.h"

CUserWarriorAnimController::CUserWarriorAnimController(CSHPTRREF<UDevice> _spDevice) 
	: UAnimationController(_spDevice), m_iWComboStack{ 0 }, m_iSComboStack{ 0 }
{
}

CUserWarriorAnimController::CUserWarriorAnimController(const CUserWarriorAnimController& _rhs) : 
	UAnimationController(_rhs), m_iWComboStack{ 0 }, m_iSComboStack{ 0 }
{
}

void CUserWarriorAnimController::Free()
{
}

HRESULT CUserWarriorAnimController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CUserWarriorAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    SHPTR<UCharacter> spCharacter = GetOwnerCharacter();
	m_wpWarriorPlayer = std::dynamic_pointer_cast<CWarriorPlayer>(spCharacter);
   
	return S_OK;
}

void CUserWarriorAnimController::Tick(const _double& _dTimeDelta)
{
    ClearTrigger();
    SetAnimState(-1);

    SHPTR<CWarriorPlayer> spWarriorPlayer = m_wpWarriorPlayer.lock();
    SHPTR<UAnimModel> spAnimModel = spWarriorPlayer->GetAnimModel();
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    // Input checks
    _bool isRunshift = spGameInstance->GetDIKeyPressing(DIK_LSHIFT);
    _bool isMoveFront = spGameInstance->GetDIKeyPressing(DIK_W);
    _bool isMoveBack = spGameInstance->GetDIKeyPressing(DIK_S);
    _bool isMoveLeft = spGameInstance->GetDIKeyPressing(DIK_A);
    _bool isMoveRight = spGameInstance->GetDIKeyPressing(DIK_D);
    _bool isWAttack = spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L);
    _bool isSAttack = spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_R);
    _bool isRAttack = spGameInstance->GetDIKeyDown(DIK_Q);
    _bool isCombo1 = spGameInstance->GetDIKeyDown(DIK_1);
    _bool isCombo2 = spGameInstance->GetDIKeyDown(DIK_2);
    _bool isAttack = isWAttack || isRAttack || isSAttack || isCombo1 || isCombo2;
    _bool isRoll = spGameInstance->GetDIKeyDown(DIK_C);
    _bool Hit = false;
    if (spWarriorPlayer->GetPrevHealth() > spWarriorPlayer->GetHealth())
        Hit = true;
    _bool isKicked = spWarriorPlayer->GetKickedState();
    _bool isJump = spGameInstance->GetDIKeyDown(DIK_SPACE);
    _bool isRise = spWarriorPlayer->GetRiseState();

    _float KickedTimeElapsed = spWarriorPlayer->GetWarriorKickedTimeElapsed();

    _double KickedAnimSpeed = 20;
    _double KickedTimeArcOpenEnd = 40;

    // Idle state check
    if (CurAnimName != L"rise01" || !isRoll || !isKicked || !isJump || !isRise && !isMoveFront && !isMoveBack && !isMoveLeft && !isMoveRight && !isAttack && !isCombo1 && !isCombo2) {
        UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
    }
    else if (GetAnimState() == ANIM_ATTACK) {
        UpdateState(spAnimModel, ANIM_IDLE, L"IDLE");
        return;
    }

    // Movement handling
    auto updateMovement = [&](const wchar_t* walkAnim, const wchar_t* runAnim, _bool isMove) {
        if (isMove) {
            if (!isRunshift) {
                UpdateState(spAnimModel, ANIM_MOVE, walkAnim);
            }
            else {
                UpdateState(spAnimModel, ANIM_MOVE, runAnim);
            }
        }
        };

    updateMovement(L"WALKF", L"RUNF", isMoveFront);
    updateMovement(L"WALKB", L"RUNB", isMoveBack);
    updateMovement(L"WALKL", L"RUNL", isMoveLeft);
    updateMovement(L"WALKR", L"RUNR", isMoveRight);

    if (isMoveFront && isMoveLeft) updateMovement(L"WALKFL", L"RUNFL", true);
    if (isMoveFront && isMoveRight) updateMovement(L"WALKFR", L"RUNFR", true);
    if (isMoveBack && isMoveLeft) updateMovement(L"WALKBL", L"RUNBL", true);
    if (isMoveBack && isMoveRight) updateMovement(L"WALKBR", L"RUNBR", true);

    // Jump or fall state
    if (spWarriorPlayer->GetJumpingState() || spWarriorPlayer->GetFallingState()) {
        UpdateState(spAnimModel, ANIM_JUMP, L"JUMP");
        m_iWComboStack = 0;
        m_iSComboStack = 0;
        spWarriorPlayer->GetTransform()->MoveForward(_dTimeDelta, isRunshift ? 30.f : 10.f);
    }

    // Jumping logic
    if (!isAttack && !isMoveBack && isJump) {
        if (CurAnimName != L"hit_head_back" && CurAnimName != L"dead01" && CurAnimName != L"rise01" && CurAnimName != L"rise02"
            && CurAnimName != L"combo02_1" && CurAnimName != L"combo02_2" && CurAnimName != L"combo02_3"
            && CurAnimName != L"combo06_1" && CurAnimName != L"combo06_2" && CurAnimName != L"combo06_3"
            && CurAnimName != L"combo05" && CurAnimName != L"combo09" && CurAnimName != L"attack04")
        {
            if (!spWarriorPlayer->GetJumpingState() && !spWarriorPlayer->GetFallingState())
            {
                spWarriorPlayer->SetJumpingState(true);
            }
        }
    }

    // Roll handling
    if (isRoll) {
        if (isMoveFront) UpdateState(spAnimModel, ANIM_ROLL, L"ROLL_F");
        else if (isMoveBack) UpdateState(spAnimModel, ANIM_ROLL, L"ROLL_B");
        else if (isMoveLeft) UpdateState(spAnimModel, ANIM_ROLL, L"ROLL_L");
        else if (isMoveRight) UpdateState(spAnimModel, ANIM_ROLL, L"ROLL_R");
        else UpdateState(spAnimModel, ANIM_ROLL, L"ROLL_F");
    }

    // Hit state
    if (Hit && !isKicked) {
        HitStack++;
        UpdateState(spAnimModel, ANIM_HIT, L"HIT_BACK");
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
        spWarriorPlayer->SetPrevHealth(spWarriorPlayer->GetHealth());
    }

    // Rise state
    if (isRise) {
         UpdateState(spAnimModel, ANIM_MOVE, L"RISE1");
        spAnimModel->SetAnimation(L"rise01");
    }
    if (CurAnimName == L"rise01")
        spWarriorPlayer->SetRiseState(false);

    // Kicked state
    if (isKicked && CurAnimName != L"rise01") {
        UpdateState(spAnimModel, ANIM_HIT, L"GOTKICKED");
        HitStack++;
        spAnimModel->UpdateAttackData(false, spAnimModel->GetAttackCollider());
    }

    // Attack handling
    if (isAttack && !Hit) {
        if (isWAttack) {
            spWarriorPlayer->SetAttack(50);
            if (CurAnimName == L"combo02_1") m_iWComboStack = 2;
            else if (CurAnimName == L"combo02_2") m_iWComboStack = 3;
            else m_iWComboStack = 1;

            switch (m_iWComboStack) {
            case 1: UpdateState(spAnimModel, ANIM_ATTACK, L"WATTACK1"); break;
            case 2: UpdateState(spAnimModel, ANIM_ATTACK, L"WATTACK2"); break;
            case 3: UpdateState(spAnimModel, ANIM_ATTACK, L"WATTACK3"); break;
            }
        }
        else if (isSAttack) {
            spWarriorPlayer->SetAttack(50);
            if (CurAnimName == L"combo06_1") m_iSComboStack = 2;
            else if (CurAnimName == L"combo06_2") m_iSComboStack = 3;
            else m_iSComboStack = 1;

            switch (m_iSComboStack) {
            case 1: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK1"); break;
            case 2: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK2"); break;
            case 3: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK3"); break;
            }
        }
        else if (isRAttack&& ShortAttackisAvailable&& UltimateAttackOneDurationTime< 0.0001f && UltimateAttackTwoDurationTime< 0.0001f) {
            ShortAttackCoolTime = Q_SKILL;
            ShortAttackDurationTime = 1.f;
            UpdateState(spAnimModel, ANIM_ATTACK, L"RATTACK");
            m_iWComboStack = 0;
            m_iSComboStack = 0;
        }
        else if (isCombo1&& UltAttackOneisAvailable&& UltimateAttackTwoDurationTime<0.0001f&& ShortAttackDurationTime< 0.0001f) {
            UltimateAttackOneCoolTime = ONE_SKILL;
            UltimateAttackOneDurationTime = 2.25f;
            UpdateState(spAnimModel, ANIM_ATTACK, L"COMBO5");
            m_iWComboStack = 0;
            m_iSComboStack = 0;
        }
        else if (isCombo2&& UltAttackTwoisAvailable&& ShortAttackDurationTime< 0.0001f && UltimateAttackOneDurationTime< 0.0001f) {
            UltimateAttackTwoCoolTime = TWO_SKILL;
            UltimateAttackTwoDurationTime = 2.5f;
            UpdateState(spAnimModel, ANIM_ATTACK, L"COMBO9");
            m_iWComboStack = 0;
            m_iSComboStack = 0;
        }

    }

    if (UltimateAttackOneCoolTime > 0.f) {
        UltAttackOneisAvailable = false;
       
        UltimateAttackOneDurationTime -= _dTimeDelta;
        if (UltimateAttackOneDurationTime <= 0)
        {
            UltimateAttackOneCoolTime -= _dTimeDelta;
        }
    }
    else {
        UltAttackOneisAvailable = true;
    }

    if (UltimateAttackTwoCoolTime > 0.f) {
        UltAttackTwoisAvailable = false;
        UltimateAttackTwoDurationTime -= _dTimeDelta;
        if (UltimateAttackTwoDurationTime <= 0)
        {
            UltimateAttackTwoCoolTime -= _dTimeDelta;
        }
    }
    else {
        UltAttackTwoisAvailable = true;
    }
    if (ShortAttackCoolTime > 0.f) {
        ShortAttackisAvailable = false;
        ShortAttackDurationTime -= _dTimeDelta;
        if (ShortAttackDurationTime <= 0)
        {
            ShortAttackCoolTime -= _dTimeDelta;
        }
      
    }
    else {
        ShortAttackisAvailable = true;
    }
    if (HitStack >= 5) {
        HitStack = 0;
        BlindEffectOn = true;
    }
    else {
        BlindEffectOn = false;
    }

    // Combo attack state
    if (CurAnimName == L"attack04" || CurAnimName == L"combo09" || CurAnimName == L"combo05" || CurAnimName == L"combo06_1" || CurAnimName == L"combo06_2" || CurAnimName == L"combo06_3" || CurAnimName == L"combo02_1" || CurAnimName == L"combo02_2" || CurAnimName == L"combo02_3") {
        spWarriorPlayer->IfAttack(true);
    }
    else {
        spWarriorPlayer->IfAttack(false);
    }

    if (true == spGameInstance->IsMouseInWindowSize())
    { 
        // Mouse Move
        _long		MouseMove = spGameInstance->GetDIMMoveState(DIMM_X);
        if (MouseMove)
        {
            if(CurAnimName != L"dead01")
            {
                spWarriorPlayer->GetTransform()->RotateTurn(_float3(0.f, 1.f, 0.f), MouseMove * 5.f, _dTimeDelta);
            }
        }
    }

   // Tick eve
   spAnimModel->TickEvent(spWarriorPlayer.get(), GetTrigger(), _dTimeDelta);


   //if Kicked & Stunned
   if (isKicked)
   {
       spWarriorPlayer->SetWarriorKickedTimeElapsed(KickedTimeElapsed + _dTimeDelta * KickedAnimSpeed);
       if (KickedTimeElapsed < KickedTimeArcOpenEnd)
           spAnimModel->TickAnimToTimeAccChangeTransform(spWarriorPlayer->GetTransform(), _dTimeDelta, KickedTimeElapsed);
       if (KickedTimeElapsed >= 50)
           if (spGameInstance->GetDIKeyDown(DIK_SPACE))
           {
               KickedTimeElapsed = 0;
               spWarriorPlayer->SetKickedState(false);
               spWarriorPlayer->SetRiseState(true);
           }
   }
   else
     spAnimModel->TickAnimChangeTransform(spWarriorPlayer->GetTransform(), _dTimeDelta);

#ifdef _ENABLE_PROTOBUFF
   _int NetworkID = spGameInstance->GetNetworkOwnerID();
   PLAYERSTATE csPlayerState;
   PROTOFUNC::MakePlayerState(OUT& csPlayerState, NetworkID, isAttack,
       GetAnimState(), isRunshift ? 30.f : 10.f, spAnimModel->GetCurrentAnimation()->GetDuration(),
       spAnimModel->GetCurrentAnimIndex());
   {
       spGameInstance->SendProcessPacket(UProcessedData(csPlayerState, TAG_CS_PLAYERSTATE));
   }
#endif
}


void CUserWarriorAnimController::ReceiveNetworkProcessData(void* _pData)
{

}


