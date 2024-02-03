#pragma once

#include "TImGuiView.h"

/*
@ Date: 2024-02-03, Writer: 이성현
@ Explain
- Imgui의 메인 뷰 
*/

BEGIN(Tool)

class TMainView : public TImGuiView {
public:
	TMainView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TMainView)
		DESTRUCTOR(TMainView)
public:
	// TImGuiView을(를) 통해 상속됨
	virtual void Free() override;

	virtual HRESULT NativeConstruct() override;
	void InsertImGuiView(const CSHPTRREF<TImGuiView> _spImGuiView);
protected:
	virtual HRESULT LoadResource() override;
	virtual HRESULT ReleaseResource() override;
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDetla) override;
	virtual void RenderActive() override;
private:
	void RenderMenu();
private:
	MAINDESC													m_stMainDesc;
	DOCKDESC												m_stDebuggingView;

	VECTOR<SHPTR<TImGuiView>>			m_vecImGuies;
	VECTOR<SHPTR<TImGuiView>>			m_vecOpenImGuies;

	_double														m_dShowDeltaTime;
	_bool															m_isDockBuilding;
};

END