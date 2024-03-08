#pragma once

#include "TImGuiView.h"

BEGIN(Engine)
class UStageManager;
class URegion;
END

BEGIN(Tool)
class TNavigationView : public TImGuiView {
public:
	TNavigationView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TNavigationView)
	DESTRUCTOR(TNavigationView)
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
	void RenderMenu();
	void DockBuildInitSetting();

	void NavigationView();
	void ModifyNavigation(CSHPTRREF<URegion> _spRegion);


private:
	MAINDESC												m_stMainDesc;
	DOCKDESC												m_stNavigationView;

	VECTOR<SHPTR<TImGuiView>>								m_CloseImGuies;
	SET<SHPTR<TImGuiView>>									m_OpenImGuies;

	_double													m_dShowDeltaTime;
	_bool													m_isInitSetting;

private:
	_bool													m_bNavigationModify;
	_bool													m_bAllRender;
	_bool													m_bRenderWireFrame;
	_bool													m_bNavigationDebugColor;

	_uint													m_iCreateRegionIndex;
	_uint													m_iRegionIndex;

};
END