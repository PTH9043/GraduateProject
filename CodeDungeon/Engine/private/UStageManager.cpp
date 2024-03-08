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
	return _bool();
}

_bool UStageManager::Save(const _wstring& _wstrPath)
{
	return _bool();
}
