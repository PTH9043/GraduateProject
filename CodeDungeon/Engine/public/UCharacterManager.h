#pragma once

#include "UBase.h"

BEGIN(Engine)
class UCharacter;

using PLAYERLIST = VECTOR<SHPTR<UCharacter>>;

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