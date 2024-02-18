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

	void AnimChangesBetweenShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags,  const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent);
	void AnimOccursTimePassShow(CSHPTRREF<UAnimation> _spAnim, ImGuiTableFlags _flags, const VECTOR<SHPTR<UAnimEvent>>& _AnimEvent);
	// 공통된 ImGui 창들을 여기에 기입 (키보드, 마우스 입력에 관한 ImGui)
	void AnimInputShow(ANIMEVENTDESC* _pEvenetDesc, const _string& _strIndex);
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

	static constexpr	_int							 KEYPRESSTAG_CNT{ 4 };
	static constexpr	_int							 KEYBOARDTAG_CNT{ 145 };
	static constexpr  _int							 MOUSETAG_CNT{ 3 };
	static constexpr	_int							 MKTAG_CNT{ 3 };

	static const _char*								KEYPRESSTAG[];
	static const _char*								KEYTAG[];
	static const _char*								MOUSETAG[];
	static const _char*								MKTAG[];

	static 	const _char*								s_AnimTags[1000];
};

END