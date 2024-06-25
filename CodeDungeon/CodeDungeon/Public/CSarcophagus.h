#pragma once
#include "CMob.h"

BEGIN(Client)

class CSarcophagus final : public CMob {
public:
	CSarcophagus(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CSarcophagus(const CSarcophagus& _rhs);
	enum SARCOTYPE {
		TYPE_STANDING, TYPE_LYING, TYPE_END
	};
	DESTRUCTOR(CSarcophagus)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CSarcophagus, "CSarcophagus::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;

	void SetSarcophagusType(SARCOTYPE _type) { m_SarcophagusType = _type; }
	SARCOTYPE GetSarcophagusType() { return m_SarcophagusType; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;

private:
	SARCOTYPE m_SarcophagusType;

};

END

