#include "EngineDefine.h"
#include "UCharacterManager.h"

UCharacterManager::UCharacterManager() : m_spCurrentPlayer{nullptr}
{
}

void UCharacterManager::ReigsterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer)
{
	assert(nullptr != _spCurrentPlayer);
	m_spCurrentPlayer = _spCurrentPlayer;
}

void UCharacterManager::Free()
{
}
