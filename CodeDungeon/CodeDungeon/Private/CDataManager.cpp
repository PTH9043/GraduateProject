#include "ClientDefines.h"
#include "UGameInstance.h"
#include "CDataManager.h"
#include "CModelManager.h"

CDataManager::CDataManager() : m_spModelManager{ nullptr }
{
}

void CDataManager::Free()
{
	m_spModelManager.reset();
}

HRESULT CDataManager::NativeConstruct()
{
	m_spModelManager = Create<CModelManager>();

	return S_OK;
}

HRESULT CDataManager::Load_Data()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spModelManager->CreateModelProtos(spGameInstance, spGameInstance->GetDevice());

	return S_OK;
}
