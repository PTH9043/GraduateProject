#pragma once
#include "UPawn.h"

BEGIN(Engine)
class UModel;
END

BEGIN(Tool)
/*
@ Date: 2024-02-08, Writer: 이성현
@ Explain
- 맵 오브젝트들을 보여주기 위한 클래스
*/
class TShowMapObjects final : public UPawn {
public:
	TShowMapObjects(CSHPTRREF<UDevice> _spDevice,
		const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TShowMapObjects(const TShowMapObjects& _rhs);
	DESTRUCTOR(TShowMapObjects)
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(TShowMapObjects, "TShowMapObjects::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void SetShowModel(CSHPTRREF<UModel> _spModel) { m_spModel = _spModel; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, 
		CSHPTRREF<UTableDescriptor> _spTableDescripto) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UModel>				m_spModel;
};

END