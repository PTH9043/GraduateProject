#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWANIMMODELOBJECT_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWANIMMODELOBJECT_H

#include "UCharacter.h"

BEGIN(Engine)
class UAnimModel;
END

BEGIN(Tool)

class TShowAnimModelObject : public UCharacter {
public:
	TShowAnimModelObject(CSHPTRREF<UDevice> _spDevice, 	const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TShowAnimModelObject(const TShowAnimModelObject& _rhs);
	DESTRUCTOR(TShowAnimModelObject)
public:
	CSHPTRREF<UAnimModel> GetAnimModel() const { return m_spModel; }
	
public:
		// UPawn을(를) 통해 상속됨
	CLONE_MACRO(TShowAnimModelObject, "TShowAnimModelObject::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void SetShowModel(CSHPTRREF<UAnimModel> _spModel) { m_spModel = _spModel; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UAnimModel>				m_spModel;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWANIMMODELOBJECT_H