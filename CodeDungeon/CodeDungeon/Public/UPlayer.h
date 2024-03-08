#pragma once

#include "UCharacter.h"

BEGIN(Engine)

END

BEGIN(Client)

class UPlayer final : public UCharacter {
public:
	UPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UPlayer(const UPlayer& _rhs);
	DESTRUCTOR(UPlayer)
public:
	// UCharacter을(를) 통해 상속됨
	CLONE_MACRO(UPlayer, "UPlayer::CloneToFailed")
	void Free() override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
protected:
	void TickActive(const _double& _dTimeDelta) override;
	void LateTickActive(const _double& _dTimeDelta) override;
	HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	void Collision(CSHPTRREF<UPawn> _pEnemy) override;
};

END

