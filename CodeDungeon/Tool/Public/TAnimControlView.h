#pragma once

#include "TImGuiView.h"

BEGIN(Engine)
class UAnimModel;
class UAnimation;
class UAnimEvent;
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
	void AnimModifyView();
private:
	void MakeAnimEvent();

	void AnimSectionShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags,  const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent);
	void AnimOccursShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent);
	void AnimColliderShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent);
private:
	MAINDESC												m_stMainDesc;
	DOCKDESC											m_stAnimModelSelectDesc;
	DOCKDESC											m_stAnimModifyDesc;
	_bool														m_isInitSetting;

	SHPTR<TAnimControlModel>			m_spAnimControlModel;
	SHPTR<UAnimModel>						m_spShowAnimModel;
	ANIMFILECONTAINER							m_AnimFileContainer;
	SHPTR<FILEGROUP>							m_spSelectAnimFileFolder;
	SHPTR<FILEDATA>								m_spSelectAnimFileData;

	SHPTR<UAnimation>							m_spSelectAnim;
	_int															m_iSelectAnimEvent;
	_int															m_AnimMaxTagCount;

	static 	const _char*								s_AnimTags[1000];
};

END