#pragma once

#include "TImGuiView.h"

BEGIN(Engine)
class UAnimModel;
END

BEGIN(Tool)
class TShowAnimModelObject;
class TAnimControlModel;

using ANIMFILECONTAINER = UNORMAP<_string, SHPTR<FILEDATA>>;

/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- Imgui Animation Tool
*/
class TAnimControlView : public TImGuiView {
public:
	TAnimControlView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TAnimControlView)
	DESTRUCTOR(TAnimControlView)
public:
	// TImGuiView을(를) 통해 상속됨
	virtual void Free() override;

	virtual HRESULT NativeConstruct() override;
protected:
	virtual HRESULT LoadResource() override;
	virtual HRESULT ReleaseResource() override;
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDetla) override;
	virtual void RenderActive() override;
private:
	void DockBuildInitSetting();
	void AnimModelSelectView();
	void AnimControlView();
	void AnimModifyView();
private:
	MAINDESC												m_stMainDesc;
	DOCKDESC											m_stAnimModelSelectDesc;
	DOCKDESC											m_stAnimControlDesc;
	DOCKDESC											m_stAnimModifyDesc;
	_bool														m_isInitSetting;

	SHPTR<TAnimControlModel>			m_spAnimControlModel;
	SHPTR<UAnimModel>						m_spShowAnimModel;
	ANIMFILECONTAINER							m_AnimFileContainer;
	SHPTR<FILEGROUP>							m_spSelectAnimFileFolder;
	SHPTR<FILEDATA>								m_spSelectAnimFileData;
};

END