#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;
class UPawn;
class UPlayer;

using COLLISIONPAWNLIST = SET<SHPTR<UPawn>>;
using PLAYERCONTAINER = SET<SHPTR<UPlayer>>;

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
	const PLAYERCONTAINER& GetPlayerContainer() const { return m_PlayerContainer; }

	void RegisterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer);
	void AddCollisionPawnList(CSHPTRREF<UPawn> _spPawn);
	void AddPlayerContainer(CSHPTRREF<UPlayer> _spPlayer);
	void RemoveCollisionPawn(CSHPTRREF<UPawn> _spPawn);
	SHPTR<UPlayer> FindPlayerToDistance(const _float3& _vPos);

	SHPTR<UPlayer> FindPlayerToNetworkID(const _int _iNetworkID);

	void TickCollider(const _double& _dTimeDelta);
	void ClearData();
private:
	virtual void Free() override;
private:
	SHPTR<UCharacter>		m_spCurrentPlayer;
	PLAYERCONTAINER		m_PlayerContainer;
	COLLISIONPAWNLIST		m_CollisionPawnList;
};

END