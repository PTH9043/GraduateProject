#pragma once
#include "CMob.h"

BEGIN(Client)

class CItemChest final : public CMob {
public:
	CItemChest(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CItemChest(const CItemChest& _rhs);
	DESTRUCTOR(CItemChest)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CItemChest, "CItemChest::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	void SetOpeningState(const _bool& _newState) { m_bisOpen = _newState; }

protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;

private:
	_bool			m_bisOpen;
};

END