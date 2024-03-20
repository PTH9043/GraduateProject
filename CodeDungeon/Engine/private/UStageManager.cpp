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
	if (nullptr != m_spStage)
	{
		m_spStage.reset();
		m_spStage = CreateConstructorNative<UStage>(spGameInstance->GetDevice());
	}
	else
	{
		m_spStage = CreateConstructorNative<UStage>(spGameInstance->GetDevice());
	}

	m_spStage->Load(_wstrPath);
	m_spStage->AddArroundRegion();

	return true;
}

_bool UStageManager::Save(const _wstring& _wstrPath)
{
	RETURN_CHECK(nullptr == m_spStage, false)
	_wstring str;
	str.assign(_wstrPath.begin(), _wstrPath.end());
	str.append(L"\\Navigation");
	m_spStage->Save(str.c_str());

	return true;
}
