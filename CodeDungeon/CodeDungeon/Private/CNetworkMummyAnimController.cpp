#include "ClientDefines.h"
#include "CNetworkMummyAnimController.h"
#include "CMummy.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UPlayer.h"
#include "USound.h"

CNetworkMummyAnimController::CNetworkMummyAnimController(CSHPTRREF<UDevice> _spDevice) 
	: CMonsterAnimController(_spDevice),
    m_pAttack1Channel{nullptr}, m_pAttack2Channel{nullptr},m_pSwhoosh1Channel{nullptr},
m_pSwhoosh2Channel{nullptr}, m_pTauntChannel{nullptr}, m_iRandomNumforhit{0},
m_pHitChannel{nullptr}, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{false}, m_isPlayHitSound{false}
{
}

CNetworkMummyAnimController::CNetworkMummyAnimController(const CNetworkMummyAnimController& _rhs)
    : CMonsterAnimController(_rhs),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

void CNetworkMummyAnimController::Free()
{
}

HRESULT CNetworkMummyAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CNetworkMummyAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpMummyMob = std::dynamic_pointer_cast<CMummy>(GetOwnerCharacter());

    return S_OK;
}

void CNetworkMummyAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    SHPTR<CMummy> spMummy = m_wpMummyMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMummy->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _bool isHit = spMummy->IsDamaged();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis_hit(0, 2);

    static const _tchar* TAUNTANIM = L"taunt";

    static const _tchar* TAUNTV02_SOUNDNAME = L"Taunt_VO2";
    static const _tchar* ENEMYHIT1_SOUNDNAME = L"enemy_hit1";
    static const _tchar* ENEMYHIT2_SOUNDNAME = L"enemy_hit2";
    static const _tchar* ENEMYHIT3_SOUNDNAME = L"enemy_hit3";
    static const _tchar* GOTHITVO2_SOUNDNAME = L"GotHit_VO_2";


    if (TAUNTANIM == CurAnimName)
    {
        SHPTR<USound> spTauntSound1 = spGameInstance->BringSound(TAUNTV02_SOUNDNAME);
        if (false == spTauntSound1->IsSoundPlay(m_pTauntChannel))
        {
            spTauntSound1->PlayWithInputChannel(&m_pTauntChannel);
        }
    }

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

            spHitSound->PlayWithInputChannel(&m_pHitChannel);
        }
        else
        {
            if (false == spHitSound->IsSoundPlay(m_pHitChannel))
            {
                spHitSound->PlayWithInputChannel(&m_pHitChannel);
            }
            else
            {
                if (0 == Random)
                    spHitSound = spGameInstance->BringSound(ENEMYHIT1_SOUNDNAME);
                else if (1 == Random)
                    spHitSound = spGameInstance->BringSound(ENEMYHIT2_SOUNDNAME);
                else
                    spHitSound = spGameInstance->BringSound(ENEMYHIT3_SOUNDNAME);

                spHitSound->PlayWithInputChannel(&m_pHitChannel);
            }
        }

        SHPTR<USound> spGotHitSound = spGameInstance->BringSound(GOTHITVO2_SOUNDNAME);

        if (false == spGotHitSound->IsSoundPlay(m_pGotHitChannel))
            spGotHitSound->PlayWithInputChannel(&m_pGotHitChannel);
    }


    spAnimModel->TickEventToRatio(spMummy.get(), GetTrigger(), GetTimeAccToNetworkAnim(), _dTimeDelta);
    spMummy->SetDamaged(false);
}
