#include "EngineDefine.h"
#include "UStageManager.h"
#include "UStage.h"
#include "UGameInstance.h"

UStageManager::UStageManager()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spStage = CreateConstructorNative<UStage>(spGameInstance->GetDevice());
}

void UStageManager::Free()
{
}

_bool UStageManager::Load(const _wstring& _wstrPath)
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spStage.reset();
	m_spStage = CreateConstructorNative<UStage>(spGameInstance->GetDevice());
	m_spStage->Load(_wstrPath);
	m_spStage->AddArroundRegion();

	return true;
}

_bool UStageManager::Save(const _wstring& _wstrPath)
{
	RETURN_CHECK(nullptr == m_spStage, false)

	m_spStage->Save(_wstrPath);

	return true;
}
