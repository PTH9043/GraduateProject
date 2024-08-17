#include "ClientDefines.h"
#include "CNetworkMinotaurAnimController.h"
#include "CMinotaur.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UPlayer.h"
#include "CWarriorPlayer.h"

CNetworkMinotaurAnimController::CNetworkMinotaurAnimController(CSHPTRREF<UDevice> _spDevice)
    : CMonsterAnimController(_spDevice),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

CNetworkMinotaurAnimController::CNetworkMinotaurAnimController(const CNetworkMinotaurAnimController& _rhs)
    : CMonsterAnimController(_rhs),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

void CNetworkMinotaurAnimController::Free()
{
}

HRESULT CNetworkMinotaurAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CNetworkMinotaurAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpMinotaurMob = std::dynamic_pointer_cast<CMinotaur>(GetOwnerCharacter());

    return S_OK;
}

void CNetworkMinotaurAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis_hit(0, 2);

    static const _tchar* ENEMYHIT1_SOUNDNAME = L"enemy_hit1";
    static const _tchar* ENEMYHIT2_SOUNDNAME = L"enemy_hit2";
    static const _tchar* ENEMYHIT3_SOUNDNAME = L"enemy_hit3";
    static const _tchar* GOTHITVO2_SOUNDNAME = L"GotHit_VO_2";


    SHPTR<CMinotaur> spMino = m_wpMinotaurMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMino->GetAnimModel();
    SHPTR<UPlayer> spPlayer = spMino->GetTargetPlayer();
    _bool isHit = spMino->IsDamaged();
    if (true == isHit)
    {
        _int Random = dis_hit(gen);

        SHPTR<USound> spHitSound = nullptr;
        if (nullptr == spHitSound)
        {
            if (0 == Random)
                spHitSound = spGameInstance->BringSound(ENEMYHIT1_SOUNDNAME);
            else if (1 == Random)
                spHitSound = spGameInstance->BringSound(ENEMYHIT2_SOUNDNAME);
            else
                spHitSound = spGameInstance->BringSound(ENEMYHIT3_SOUNDNAME);

            spHitSound->PlayWithInputChannel(&m_pHitChannel, spMino->GetTransform(), spPlayer->GetTransform());
        }
        else
        {
            if (false == spHitSound->IsSoundPlay(m_pHitChannel))
            {
                spHitSound->PlayWithInputChannel(&m_pHitChannel, spMino->GetTransform(), spPlayer->GetTransform());
            }
            else
            {
                if (0 == Random)
                    spHitSound = spGameInstance->BringSound(ENEMYHIT1_SOUNDNAME);
                else if (1 == Random)
                    spHitSound = spGameInstance->BringSound(ENEMYHIT2_SOUNDNAME);
                else
                    spHitSound = spGameInstance->BringSound(ENEMYHIT3_SOUNDNAME);

                spHitSound->PlayWithInputChannel(&m_pHitChannel, spMino->GetTransform(), spPlayer->GetTransform());
            }
        }

        SHPTR<USound> spGotHitSound = spGameInstance->BringSound(GOTHITVO2_SOUNDNAME);

        if (false == spGotHitSound->IsSoundPlay(m_pGotHitChannel))
            spGotHitSound->PlayWithInputChannel(&m_pGotHitChannel, spMino->GetTransform(), spPlayer->GetTransform());
    }


    spAnimModel->TickEventToRatio(spMino.get(), GetTrigger(), GetTimeAccToNetworkAnim(), _dTimeDelta);
}

