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

HRESULT UStageManager::Load(const _wstring& _wstrPath)
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

	SHPTR<FILEGROUP> Navifolder = spGameInstance->FindFolder(L"Navigation");
	if (nullptr == Navifolder)
		return S_OK;
	for (auto& iter : Navifolder->FileDataList)
	{
		m_spStage->Load(iter.second->wstrfilePath);
	}
	m_spStage->AddArroundRegion();

	return S_OK;
}

HRESULT UStageManager::Save(const _wstring& _wstrPath)
{
	RETURN_CHECK(nullptr == m_spStage, E_FAIL)
	_wstring str;
	str.assign(_wstrPath.begin(), _wstrPath.end());
	str.append(L"\\Navigation");
	m_spStage->Save(str.c_str());

	return S_OK;
}
