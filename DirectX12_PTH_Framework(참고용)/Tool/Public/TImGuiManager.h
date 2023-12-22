#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TIMGUIMANAGER_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TIMGUIMANAGER_H

#include "UBase.h"

BEGIN(Engine)
class UDevice;
class UGpuCommand;
class UGraphicsTableDescriptor;
END

BEGIN(Tool)
class TImGuiView;
class TMainView;

class TImGuiManager : public UBase {
	DECLARE_SINGLETON(TImGuiManager)
public:
	TImGuiManager();
	DESTRUCTOR(TImGuiManager)
public:
	const _bool IsResetScene() const { return m_isResetScene; }
	void SetResetScene(const _bool _isResetScene) { this->m_isResetScene = _isResetScene; }
public:
	virtual void Free() override;

	HRESULT ReadyManager(const GRAPHICDESC& _stGraphicDesc, const OUTPUTDATA& _stOutputData);
	SHPTR<TImGuiView> GetImGuiObject(const IMGTAG _eImGuiTag);

	void Tick(const _double& _dDeltaTime);
	void LateTick(const _double& _dDeltaTime);
	void Render();
private:
	HRESULT ReadyImGuiClass();
private:
	using IMGOBJECTS = UNORMAP<IMGTAG, SHPTR<TImGuiView>>;
	// IMGUI Objects 
	IMGOBJECTS												m_uomapImGuiObjects;
	SHPTR<TMainView>								m_spMainView;

	SHPTR<UDevice>										m_spDevice;
	SHPTR<UGpuCommand>						m_spGpuCommand;
	SHPTR<UGraphicsTableDescriptor>	m_spTableDesciptor;

	_bool															m_isResetScene{ false };
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TIMGUIMANAGER_H