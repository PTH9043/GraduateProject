#include "ClientDefines.h"
#include "CNetworkHarlequinnAnimController.h"
#include "CHarlequinn.h"
#include "UGameInstance.h"
#include "UAnimModel.h"
#include "UCharacter.h"
#include "UAnimation.h"
#include "UTransform.h"
#include "UPlayer.h"
#include "CWarriorPlayer.h"
#include "CShurikenThrowing.h"

CNetworkHarlequinnAnimController::CNetworkHarlequinnAnimController(CSHPTRREF<UDevice> _spDevice)
    : CMonsterAnimController(_spDevice),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

CNetworkHarlequinnAnimController::CNetworkHarlequinnAnimController(const CNetworkHarlequinnAnimController& _rhs)
    : CMonsterAnimController(_rhs),
    m_pAttack1Channel{ nullptr }, m_pAttack2Channel{ nullptr }, m_pSwhoosh1Channel{ nullptr },
    m_pSwhoosh2Channel{ nullptr }, m_pTauntChannel{ nullptr }, m_iRandomNumforhit{ 0 },
    m_pHitChannel{ nullptr }, m_isPlayAttackSound1{ false }, m_isPlayAttackSound2{ false }, m_isPlayHitSound{ false }
{
}

void CNetworkHarlequinnAnimController::Free()
{
}

HRESULT CNetworkHarlequinnAnimController::NativeConstruct()
{
    return __super::NativeConstruct();
}

HRESULT CNetworkHarlequinnAnimController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
    RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);

    m_wpHarlequinnMob = std::dynamic_pointer_cast<CHarlequinn>(GetOwnerCharacter());

    return S_OK;
}

void CNetworkHarlequinnAnimController::Tick(const _double& _dTimeDelta)
{
    SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis_hit(0, 2);

    static const _tchar* ENEMYHIT1_SOUNDNAME = L"enemy_hit1";
    static const _tchar* ENEMYHIT2_SOUNDNAME = L"enemy_hit2";
    static const _tchar* ENEMYHIT3_SOUNDNAME = L"enemy_hit3";


    SHPTR<CHarlequinn> spHarlequinn = m_wpHarlequinnMob.lock();
    SHPTR<UAnimModel> spAnimModel = spHarlequinn->GetAnimModel();
    const _wstring& CurAnimName = spAnimModel->GetCurrentAnimation()->GetAnimName();

    _bool isHit = spHarlequinn->IsDamaged();
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

    if (CurAnimName == L"Attack 1")
    {
        m_f3ThrowingPos = spHarlequinn->GetTransform()->GetPos();
        m_f3ThrowingPos.y += 7.f;

        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.2 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.25)
        {
            for (int i = 0; i < 3; i++)
            {
                if ((*spHarlequinn->GetShurikens())[i]->GetTraveledDistance() == 0)
                {
                    if (i == 0)
                    {
                        _float3 currentLookDir = spHarlequinn->GetTransform()->GetLook();
                        _float angleRadians = 10 * (DirectX::XM_PI / 180.0f);;
                        _float cosAngle = std::cos(angleRadians);
                        _float sinAngle = std::sin(angleRadians);

                        _float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
                        _float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;


                        m_arrThrowingDir[i] = _float3(newX, currentLookDir.y, newZ);
                    }
                    else if (i == 1)
                        m_arrThrowingDir[i] = spHarlequinn->GetTransform()->GetLook();
                    else if (i == 2)
                    {
                        _float3 currentLookDir = spHarlequinn->GetTransform()->GetLook();
                        _float angleRadians = -10 * (DirectX::XM_PI / 180.0f);
                        _float cosAngle = std::cos(angleRadians);
                        _float sinAngle = std::sin(angleRadians);

                        _float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
                        _float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;


                        m_arrThrowingDir[i] = _float3(newX, currentLookDir.y, newZ);
                    }

                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetPos(m_f3ThrowingPos);
                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetDirection(spHarlequinn->GetTransform()->GetRight());
                    (*spHarlequinn->GetShurikens())[i]->SetThrow(true);
                }
            }
        }
        else if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.6 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.65)
        {
            for (int i = 3; i < 6; i++)
            {
                if ((*spHarlequinn->GetShurikens())[i]->GetTraveledDistance() == 0)
                {
                    if (i == 3)
                    {
                        _float3 currentLookDir = spHarlequinn->GetTransform()->GetLook();
                        _float angleRadians = 10 * (DirectX::XM_PI / 180.0f);
                        _float cosAngle = std::cos(angleRadians);
                        _float sinAngle = std::sin(angleRadians);

                        _float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
                        _float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;


                        m_arrThrowingDir[i] = _float3(newX, currentLookDir.y, newZ);
                    }
                    else if (i == 4)
                        m_arrThrowingDir[i] = spHarlequinn->GetTransform()->GetLook();
                    else if (i == 5)
                    {
                        _float3 currentLookDir = spHarlequinn->GetTransform()->GetLook();
                        _float angleRadians = -10 * (DirectX::XM_PI / 180.0f);;
                        _float cosAngle = std::cos(angleRadians);
                        _float sinAngle = std::sin(angleRadians);

                        _float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
                        _float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;


                        m_arrThrowingDir[i] = _float3(newX, currentLookDir.y, newZ);
                    }
                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetPos(m_f3ThrowingPos);
                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetDirection(spHarlequinn->GetTransform()->GetRight());
                    (*spHarlequinn->GetShurikens())[i]->SetThrow(true);
                }
            }
        }
    }

    if (CurAnimName == L"Attack 4")
    {
        m_f3ThrowingPos = spHarlequinn->GetTransform()->GetPos();
        m_f3ThrowingPos.y += 7.f;
        if (spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() >= 0.2 && spAnimModel->GetCurrentAnimation()->GetAnimationProgressRate() < 0.25)
        {
            for (int i = 0; i < 6; i++)
            {
                _float angle = 0;
                if ((*spHarlequinn->GetShurikens())[i]->GetTraveledDistance() == 0)
                {
                    if (i == 0)
                    {
                        m_arrThrowingDir[i] = spHarlequinn->GetTransform()->GetLook();
                    }
                    else
                    {
                        angle = 60 * (i + 1);
                        _float3 currentLookDir = spHarlequinn->GetTransform()->GetLook();
                        _float angleRadians = angle * (DirectX::XM_PI / 180.0f);
                        _float cosAngle = std::cos(angleRadians);
                        _float sinAngle = std::sin(angleRadians);

                        _float newX = currentLookDir.x * cosAngle + currentLookDir.z * sinAngle;
                        _float newZ = -currentLookDir.x * sinAngle + currentLookDir.z * cosAngle;


                        m_arrThrowingDir[i] = _float3(newX, currentLookDir.y, newZ);
                    }

                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetPos(m_f3ThrowingPos);
                    (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetDirection(spHarlequinn->GetTransform()->GetRight());
                    (*spHarlequinn->GetShurikens())[i]->SetThrow(true);
                }
            }
        }
    }

    //½´¸®ÄË ´øÁö±â 
    for (int i = 0; i < 6; i++)
    {
        if ((*spHarlequinn->GetShurikens())[i]->GetTraveledDistance() > 100)
        {
            (*spHarlequinn->GetShurikens())[i]->SetThrow(false);
            (*spHarlequinn->GetShurikens())[i]->GetTransform()->SetPos(spHarlequinn->GetTransform()->GetPos());
            (*spHarlequinn->GetShurikens())[i]->SetTraveledDistance(0);
        }
        else
            spHarlequinn->ThrowShurikens(i, _dTimeDelta, m_arrThrowingDir[i]);
    }

    spAnimModel->TickEventToRatio(spHarlequinn.get(), GetTrigger(), GetTimeAccToNetworkAnim(), _dTimeDelta);
}
