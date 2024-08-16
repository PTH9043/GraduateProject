#include "ClientDefines.h"
#include "CNetworkMimicAnimController.h"
#include "CMimic.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UPlayer.h"
#include "UTransform.h"


CNetworkMimicAnimController::CNetworkMimicAnimController(CSHPTRREF<UDevice> _spDevice)
    : CMonsterAnimController(_spDevice),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

CNetworkMimicAnimController::CNetworkMimicAnimController(const CNetworkMimicAnimController& _rhs)
    : CMonsterAnimController(_rhs),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

void CNetworkMimicAnimController::Free()
{
}

HRESULT CNetworkMimicAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CNetworkMimicAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpMimicMob = std::dynamic_pointer_cast<CMimic>(GetOwnerCharacter());

    return S_OK;
}

void CNetworkMimicAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis_hit(0, 2);

    static const _tchar* ENEMYHIT1_SOUNDNAME = L"enemy_hit1";
    static const _tchar* ENEMYHIT2_SOUNDNAME = L"enemy_hit2";
    static const _tchar* ENEMYHIT3_SOUNDNAME = L"enemy_hit3";
    static const _tchar* GOTHITVO2_SOUNDNAME = L"GotHit_VO_2";


    SHPTR<CMimic> spMimic = m_wpMimicMob.lock();
    SHPTR<UAnimModel> spAnimModel = spMimic->GetAnimModel();

    spAnimModel->TickEventToRatio(spMimic.get(), GetTrigger(), GetTimeAccToNetworkAnim(), _dTimeDelta);
}
