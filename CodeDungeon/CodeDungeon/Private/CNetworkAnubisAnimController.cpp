#include "ClientDefines.h"
#include "CNetworkAnubisAnimController.h"
#include "CAnubis.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UGuard.h"
#include "UPlayer.h"
#include "UMat.h"

CNetworkAnubisAnimController::CNetworkAnubisAnimController(CSHPTRREF<UDevice> _spDevice)
    : CMonsterAnimController(_spDevice), m_dTimerForFireCircle{0},
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

CNetworkAnubisAnimController::CNetworkAnubisAnimController(const CNetworkAnubisAnimController& _rhs)
    : CMonsterAnimController(_rhs), m_dTimerForFireCircle{ 0 },
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

void CNetworkAnubisAnimController::Free()
{
}

HRESULT CNetworkAnubisAnimController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CNetworkAnubisAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpAnubisMob = std::dynamic_pointer_cast<CAnubis>(GetOwnerCharacter());

    return S_OK;
}

void CNetworkAnubisAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

    static const _tchar* ENEMYHIT1_SOUNDNAME = L"enemy_hit1";
    static const _tchar* ENEMYHIT2_SOUNDNAME = L"enemy_hit2";
    static const _tchar* ENEMYHIT3_SOUNDNAME = L"enemy_hit3";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis_hit(0, 2);

    SHPTR<CAnubis> spAnubis = m_wpAnubisMob.lock();
    SHPTR<UAnimModel> spAnimModel = spAnubis->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _bool isHit = spAnubis->IsDamaged();
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
    }

    if (CurAnimName == L"Attack3")
    {
        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.65 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.7)
        {
            spAnubis->SetShieldState(true);
        }
    }

    if (CurAnimName == L"Cast")
    {
        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.65 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.7)
        {
            spAnubis->SetFireAttackState(true);
        }
    }
    if (spAnubis->GetFireAttackState())
    {
        if (m_dTimerForFireCircle == 0)
        {
            spGameInstance->SoundPlayOnce(L"FireWarm");
        }
        m_dTimerForFireCircle += _dTimeDelta;
        float scale_factor = 1 + 100 * m_dTimerForFireCircle * m_dTimerForFireCircle;
        if (m_dTimerForFireCircle < 1)
        {
            spAnubis->GetFireCircle()->GetTransform()->SetScale(_float3(scale_factor, scale_factor, 2));
            spAnubis->GetFireCircle()->SetActive(true);
            spAnubis->GetFireCircle1()->GetTransform()->SetScale(_float3(scale_factor - 5, scale_factor - 5, 2));
            spAnubis->GetFireCircle1()->SetActive(true);
        }
        else
        {
            m_dTimerForFireCircle = 0;
            spAnubis->GetFireCircle()->GetTransform()->SetScale(_float3(1, 1, 1));
            spAnubis->GetFireCircle()->SetActive(false);
            spAnubis->GetFireCircle1()->GetTransform()->SetScale(_float3(1, 1, 1));
            spAnubis->GetFireCircle1()->SetActive(false);
            spAnubis->SetFireAttackState(false);
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

    // Tick event
    spAnimModel->TickEvent(spAnubis.get(), GetTrigger(), _dTimeDelta);
}
