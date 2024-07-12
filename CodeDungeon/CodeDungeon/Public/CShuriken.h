#pragma once
#include "UWeapon.h"

BEGIN(Engine)
class UGameInstance;
END

BEGIN(Client)
/*
@ Date: 2024-05-12, Writer: ¿Ãº∫«ˆ
@ Explain
- ∏ ¿« √∂¿Â
*/
class CShuriken final : public UWeapon {
public:
	CShuriken(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CShuriken(const CShuriken& _rhs);
	DESTRUCTOR(CShuriken)
public:
	// UPlayer¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	CLONE_MACRO(CShuriken, "CShuriken::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
};

END