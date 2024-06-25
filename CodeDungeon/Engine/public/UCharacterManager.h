#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;
class UPawn;

using COLLISIONPAWNLIST = SET<SHPTR<UPawn>>;
/*
@ Date: 2024-02-25, Writer: 박태현
@ Explain
- Player, Monstter, NPC등을 관리하고 그들끼리의 충돌도 관리하는 클래스이다. 
*/
class UCharacterManager final : public UBase {
public:
	UCharacterManager();
	NO_COPY(UCharacterManager)
	DESTRUCTOR(UCharacterManager)
public:
	CSHPTRREF<UCharacter> GetCurrPlayer() const { return m_spCurrentPlayer; }

	void RegisterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer);
	void AddCollisionPawnList(CSHPTRREF<UPawn> _spPawn);
	void RemoveCollisionPawn(CSHPTRREF<UPawn> _spPawn);

	void TickCollider(const _double& _dTimeDelta);
	void ClearData();
private:
	virtual void Free() override;
private:
	SHPTR<UCharacter>		m_spCurrentPlayer;
	COLLISIONPAWNLIST		m_CollisionPawnList;
};

END