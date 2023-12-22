#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWMODELOBJECT_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWMODELOBJECT_H

#include "UCharacter.h"

BEGIN(Engine)
class UModel;
END

BEGIN(Tool)

class TShowModelObject : public UPawn{
public:
	TShowModelObject(CSHPTRREF<UDevice> _spDevice, 
		const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TShowModelObject(const TShowModelObject& _rhs);
	DESTRUCTOR(TShowModelObject)
		// UPawn을(를) 통해 상속됨
	CLONE_MACRO(TShowModelObject, "TShowModelObject::Clone To Failed")
	virtual void Free() override ;
	virtual HRESULT NativeConstruct() override  ;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override ;

	void SetShowModel(CSHPTRREF<UModel> _spModel) { m_spModel = _spModel; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override  ;
	virtual void LateTickActive(const _double& _dTimeDelta) override  ;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescripto) override  ;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override  ;
private:
	SHPTR<UModel>				m_spModel;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWMODELOBJECT_H