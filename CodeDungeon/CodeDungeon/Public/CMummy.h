#pragma once
#include "UPlayer.h"

BEGIN(Client)
/*
@ Date: 2024-05-13, Writer: 이성현
@ Explain
-  미라 몬스터 클래스.
*/
class CMummy final : public UCharacter {
public:
	CMummy(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CMummy(const CMummy& _rhs);
	DESTRUCTOR(CMummy)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CMummy, "CMummy::CloneToFailed")
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

