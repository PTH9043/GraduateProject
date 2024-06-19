#include "EngineDefine.h"
#include "UCharacterManager.h"
#include "UCharacter.h"

UCharacterManager::UCharacterManager() : m_spCurrentPlayer{nullptr}
{
}

void UCharacterManager::RegisterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer)
{
	assert(nullptr != _spCurrentPlayer);
	m_spCurrentPlayer = _spCurrentPlayer;
	m_CollisionPawnList.insert(std::static_pointer_cast<UPawn>(_spCurrentPlayer));
}

void UCharacterManager::AddCollisionPawnList(CSHPTRREF<UPawn> _spPawn)
{
	assert(nullptr != _spPawn);
	m_CollisionPawnList.insert(_spPawn);
}

void UCharacterManager::RemoveCollisionPawn(CSHPTRREF<UPawn> _spPawn)
{
	m_CollisionPawnList.erase(_spPawn);
}

void UCharacterManager::TickCollider()
{
	for (auto& Pawn : m_CollisionPawnList)
	{
		for (auto& Coll : m_CollisionPawnList)
		{
			if (Pawn == Coll)
				continue;

			Pawn->IsHit(Coll);
		}
	}
}

void UCharacterManager::ClearData()
{
	m_spCurrentPlayer = nullptr;
	m_CollisionPawnList.clear();
}

void UCharacterManager::Free()
{
}
