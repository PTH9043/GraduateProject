#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCREENRENDEROBJ_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCREENRENDEROBJ_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferRect;
class URenderTargetGroup;
class UPawn;

class DEF_CACHE_ALGIN UScreenRenderObj final : public UPawn {
public:
	struct SCREENRENDEROBJDESC {
		SHPTR<UPawn> spPawn{ nullptr };
	};
public:
	UScreenRenderObj(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UScreenRenderObj(const UScreenRenderObj& _rhs);
	DESTRUCTOR(UScreenRenderObj)
public:
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(UScreenRenderObj, "UScreenRenderObj::Clone To Failed")
	void Free() override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	const _uint GetRegisterCamID() const { return m_iRegisterCamID; }

	void SetRegisterCamID(const _uint _iRegisterCamID) { this->m_iRegisterCamID = _iRegisterCamID; }

	CSHPTRREF<URenderTargetGroup> GetScreenRTGroup() const { return m_spScreenRenderTargetGroup; }
protected:
	void TickActive(const _double& _dTimeDelta) override;
	void LateTickActive(const _double& _dTimeDelta) override;
	HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	RTGROUPID										m_eScreenObjGroupID;
	_uint													m_iRegisterCamID;
	SHPTR<URenderTargetGroup>		m_spScreenRenderTargetGroup;
	SHPTR<UVIBufferRect>					m_spVIBufferRect;
	WKPTR<UPawn>								m_wpOwnerPawn;
	static	_uint										s_ScreenRenderObjCnt;
};

END

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_USCREENRENDEROBJ_H
