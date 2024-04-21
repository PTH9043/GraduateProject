#pragma once

#include "UCamera.h"

BEGIN(Engine)

class UShadowCamera : public UCamera {
public:
	UShadowCamera(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UShadowCamera(const UShadowCamera& _rhs);
	DESTRUCTOR(UShadowCamera)

public:
	virtual void Free() override;
	CLONE_MACRO(UShadowCamera, "UShadowCamera::Clone To Failed")
		virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta)  override;
};

END

