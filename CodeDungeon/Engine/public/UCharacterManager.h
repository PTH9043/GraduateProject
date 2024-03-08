#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;

using PLAYERLIST = VECTOR<SHPTR<UCharacter>>;
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

	void ReigsterCurrentPlayer(CSHPTRREF<UCharacter> _spCurrentPlayer);
private:
	virtual void Free() override;
private:
	SHPTR<UCharacter>		m_spCurrentPlayer;

	
};

END