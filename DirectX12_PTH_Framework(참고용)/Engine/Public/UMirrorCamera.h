#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRRORCAMERA_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRRORCAMERA_H

#include "UCamera.h"

BEGIN(Engine)

class DEF_CACHE_ALGIN UMirrorCamera final : public UCamera {
public:
	UMirrorCamera(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UMirrorCamera(const UMirrorCamera& _rhs);
	DESTRUCTOR(UMirrorCamera)

public:
	virtual void Free() override;
	CLONE_MACRO(UMirrorCamera, "UMirrorCamera::Clone To Failed")
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta)  override;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRRORCAMERA_H