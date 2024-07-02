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
	: UAnimationController(_spDevice), m_iWComboStack{ 0 }, m_iSComboStack{ 0 }, m_dTimeElapsed{0}
{
}

CUserWarriorAnimController::CUserWarriorAnimController(const CUserWarriorAnimController& _rhs) : 
	UAnimationController(_rhs), m_iWComboStack{ 0 }, m_iSComboStack{ 0 }, m_dTimeElapsed{0}
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

    // Input checks
    _bool isRunshift = spGameInstance->GetDIKeyPressing(DIK_LSHIFT);
    _bool isMoveFront = spGameInstance->GetDIKeyPressing(DIK_W);
    _bool isMoveBack = spGameInstance->GetDIKeyPressing(DIK_S);
    _bool isMoveLeft = spGameInstance->GetDIKeyPressing(DIK_A);
    _bool isMoveRight = spGameInstance->GetDIKeyPressing(DIK_D);
    _bool isWAttack = spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L);
    _bool isSAttack = spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_R);
    _bool isRAttack = spGameInstance->GetDIKeyDown(DIK_Q);
    _bool isAttack = isWAttack || isRAttack || isSAttack;
    _bool isCombo = spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_R);
    _bool isRoll = spGameInstance->GetDIKeyDown(DIK_C);
    _bool isHit = spWarriorPlayer->GetHitState();
    _bool isJump = spGameInstance->GetDIKeyDown(DIK_SPACE);

    // Track hit state
    static _bool hitExecuted = false;

    // Jumping logic
    if (!isAttack && !isCombo && !isMoveBack && isJump) {
        if (!spWarriorPlayer->GetJumpingState() && !spWarriorPlayer->GetFallingState()) {
            spWarriorPlayer->SetJumpingState(true);
        }
    }

    // Idle state check
    if (!isMoveFront && !isMoveBack && !isMoveLeft && !isMoveRight && !isAttack && !isCombo) {
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

    // Attack handling
    if (isAttack && !isHit) {
        const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();
        if (isWAttack) {
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
            if (CurAnimName == L"combo06_1") m_iSComboStack = 2;
            else if (CurAnimName == L"combo06_2") m_iSComboStack = 3;
            else m_iSComboStack = 1;

            switch (m_iSComboStack) {
            case 1: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK1"); break;
            case 2: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK2"); break;
            case 3: UpdateState(spAnimModel, ANIM_ATTACK, L"SATTACK3"); break;
            }
        }
        else if (isRAttack) {
            UpdateState(spAnimModel, ANIM_ATTACK, L"RATTACK");
            m_iWComboStack = 0;
            m_iSComboStack = 0;
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
    if (isHit && !hitExecuted) {
        UpdateState(spAnimModel, ANIM_HIT, L"HIT_BACK");
        hitExecuted = true; // Set the flag to indicate hit animation played
    }

    // Reset hitExecuted flag if the current animation is not HIT
    if (spAnimModel->GetCurrentAnimation()->GetAnimName() != L"HIT_BACK") {
        hitExecuted = false;
    }

    // Combo attack state
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();
    if (CurAnimName == L"combo06_1" || CurAnimName == L"combo06_2" || CurAnimName == L"combo06_3" || CurAnimName == L"combo02_1" || CurAnimName == L"combo02_2" || CurAnimName == L"combo02_3") {
        spWarriorPlayer->IfAttack(true);
    }
    else {
        spWarriorPlayer->IfAttack(false);
    }

    spWarriorPlayer->SetMouseMove(spGameInstance->GetDIMMoveState(DIMM_X));
    // Tick eve
    spAnimModel->TickEvent(spWarriorPlayer.get(), GetTrigger(), _dTimeDelta);
    spAnimModel->TickAnimChangeTransform(spWarriorPlayer->GetTransform(), _dTimeDelta);

#ifdef _ENABLE_PROTOBUFF
    _int NetworkID = spGameInstance->GetNetworkOwnerID();
    PLAYERSTATE csPlayerState;
    _string str = UMethod::ConvertWToS(GetTrigger());
    PROTOFUNC::MakePlayerState(OUT & csPlayerState, NetworkID, isAttack,
        GetAnimState(), isRunshift ? 30.f : 10.f, spAnimModel->GetCurrentAnimation()->GetDuration(), str);
    size_t value = (size_t)(str.length() * 1.5 + csPlayerState.ByteSizeLong());
    spGameInstance->SendProcessPacket(UProcessedData(csPlayerState, TAG_CS_PLAYERSTATE, value));
#endif
}


void CUserWarriorAnimController::ReceiveNetworkProcessData(void* _pData)
{

}


