#pragma once
#include "UPlayer.h"

BEGIN(Client)
/*
@ Date: 2024-04-30, Writer: 박태현
@ Explain
-  사용자가 직접 조종하는 클래스이다.
*/
class CWarriorPlayer final : public UPlayer {
public:
	CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CWarriorPlayer(const CWarriorPlayer& _rhs);
	DESTRUCTOR(CWarriorPlayer)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CWarriorPlayer, "CWarriorPlayer::CloneToFailed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	
};

END

