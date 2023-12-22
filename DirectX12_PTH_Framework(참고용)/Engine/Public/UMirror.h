#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRROR_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRROR_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferRect;
class UCamera;
class UScreenRenderObj;
class UMirrorCamera;

class DEF_CACHE_ALGIN UMirror final : public UPawn  {
public:
	UMirror(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UMirror(const UMirror& _rhs);
	DESTRUCTOR(UMirror)
private:
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(UMirror, "UMirror::Clone To Failed")
	void Free() override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void ChangeRegisterCamID(const CAMID _iCamID) { this->m_iRegisterCamID = _iCamID; }
protected:
	void TickActive(const _double& _dTimeDelta) override;
	void LateTickActive(const _double& _dTimeDelta) override;
	HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	void Collision(CSHPTRREF<UPawn> _pEnemy) override;

private:
	CAMID											m_iRegisterCamID;
	SHPTR<UVIBufferRect>				m_spVIBufferRect;
	SHPTR< UScreenRenderObj>	m_spScreenRenderObj;
	SHPTR<UMirrorCamera>			m_spMirrorCamera;
};

END

#endif //_PTH_FRAMEWORK_ENGIEN_PUBLIC_UMIRROR_H