#pragma once

#include "TImGuiView.h"

BEGIN(Engine)

END

BEGIN(Tool)
class TShowModelObject;

using MAPMODELCONTAINER = UNORMAP<_string, SHPTR<UModel>>;
using SHOWMAPMODELCONTAINER = UNORMAP<_string, SHPTR<TShowModelObject>>;
using MAPOBJECTSPOSLAYOUT = UNORMAP<_string, _float3>;

class TMapView : public TImGuiView {
	TMapView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TMapView)
	DESTRUCTOR(TMapView)
public:
	// TImGuiView을(를) 통해 상속됨
	void Free() override;
	HRESULT NativeConstruct() override;
protected:
	HRESULT LoadResource() override;
	HRESULT ReleaseResource() override;
	void TickActive(const _double& _dTimeDelta) override;
	void LateTickActive(const _double& _dTimeDetla) override;
	void DockBuildInitSetting();
	void RenderActive() override;
private:
	void MapView();

private:
	MAINDESC												m_stMainDesc;
	DOCKDESC												m_stMapView;

	_double													m_dShowDeltaTime;
	_bool													m_isInitSetting;

	MAPMODELCONTAINER										m_umMapModelContainer;
	SHOWMAPMODELCONTAINER									m_umShowMapModelContainer;

};

END