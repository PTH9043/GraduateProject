#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UShader;
class UComponent;
class UPipeLine;
class USceneManager;
class UVIBufferRect;
class UVIBufferCube;
class URenderTargetManager;
class URenderHandler;
class UDefferedCamera;
class UShadowCamera;
class UShaderConstantBuffer;
class UGraphicDevice;
class UGlobalConstantBuffer;
class UComputeManager;
class UPawn;
class URenderTargetGroup;


class URenderer final : public UComponent{
	using PAWNLIST = LIST<SHPTR<UPawn>>;
	using DRAWINGRENDERCONTAINER = UNORMAP<_wstring, PAWNLIST>;
	using SHADERODERS = UNORMAP<_wstring, SHPTR<UShader>>;
public:
#ifdef _USE_DEBUGGING
	typedef struct tagRenderDebugging
	{
		_bool		isShowRenderTarget{ false };
		_bool		isShowComponents{ false };
	}RENDERDEBUGGING;
#endif

	URenderer(CSHPTRREF <UDevice> _spDevice, CSHPTRREF<UCommand> _spCommand,
		CSHPTRREF<UGraphicDevice> _spGraphicDevice, 	CSHPTRREF<UPipeLine> _spPipeLine,
		CSHPTRREF<USceneManager> _spSceneManager, CSHPTRREF<URenderTargetManager> _spRenderTargetManager,
		CSHPTRREF<UComputeManager> _spComputeManager);
	NO_COPY(URenderer)
	DESTRUCTOR(URenderer)
public:
#ifdef _USE_DEBUGGING
	RENDERDEBUGGING& GetRenderDebugging() { return m_stRenderDebugging; }
#endif
public:
	CLONE_THIS(URenderer)
	virtual void Free() override;
public:
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vectDatas) override;

	HRESULT AddRenderGroup(const RENDERID _eID, CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn);
#ifdef _USE_DEBUGGING
	HRESULT AddDebugRenderGroup(const DEBUGRENDERID _eID,
		CSHPTRREF<UShader> _spShader, CSHPTRREF<UPawn> _spPawn);
#endif
	void Tick(const _double& _dTimeDelta);
	HRESULT Render();
	void ClearRenderingData();
	void ClearAllData();
	// Render Other Camera
	void RenderOtherCamera();
	void BindDefferedTransform(SHPTR<UShader> _spShader);
	void TurnOnFog() { m_bTurnFog = true; }
	void TurnOffFog() { m_bTurnFog = false; }
private:
	void RenderRTs();
	// Priority
	void RenderPriority();
	// Shadow
	void	RenderShadowDepth();
	// NonAlpha
	void RenderNonAlphaBlend();
	// Render Lights
	void RenderLights();
	// Render Blend
	void RenderBlend();
	// Non Light
	void RenderNonLight();
	// Alpha Blend
	void RenderAlphaBlend();
	// Distortion
	void RenderDistortion();
	// Render 3D UI
	void Render3DUI();
	// UI 
	void Render2DUI();
	// HDR
	void RenderHDR();
	// Render End 
	void RenderEnd();
#ifdef _USE_DEBUGGING
	void RenderDebug();
#endif
	SHPTR<UShader> FindShader(const _wstring& _wstrProto);
	// Draw Just Object
	void RenderObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList);
	void RenderShadowObject(const _wstring& _wstrShaderName, PAWNLIST& _PawnList);
	

	SHPTR<UShader> FrameReadyDrawFinalRenderTarget(const _wstring& _wstrShaderName, const RTGROUPID _eGroupID);
	SHPTR<UShader> FrameReadyDrawLast(const _wstring& _wstrShaderName);

	void BindGrobalBuffer();
private:
	GLOBALPARAM																				m_stGlobalParam;
	SHPTR< UGlobalConstantBuffer>											m_spGlobalBuffer;
	_float																								m_fGrobalDeltaTime;
	// Deffered Transform Param		
	TRANSFORMPARAM																	 m_stFinalRenderTransformParam;
	SHPTR<UShaderConstantBuffer>											m_spTransformConstantBuffer;
	SHPTR<UDefferedCamera>														m_spDefferedCamera;
	SHPTR<UShadowCamera>														m_spShadowCamera;
	// Actives 																							 
	ARRAY<DRAWINGRENDERCONTAINER, RI_END>					m_arrActiveDrawRenderList;
	//Shader Container 																		   
	SHADERODERS																			m_ShaderObjects;
	// Manager																						  
	SHPTR<UGraphicDevice>															m_spGraphicDevice;
	SHPTR<UPipeLine>																		m_spPipeLine;
	SHPTR<USceneManager>															m_spSceneManager;
	SHPTR<URenderTargetManager>											m_spRenderTargetManager;
	SHPTR<UComputeManager>													m_spComputeManager;
	SHPTR<UCommand>																	m_spCastingCommand;
	SHPTR<UVIBufferRect>																m_spVIBufferPlane;
	_short																								m_sNonAlphaBlendIndex;

	_bool																	m_bTurnFog;
	SHPTR<UShaderConstantBuffer>											m_spFogConstantBuffer;
#ifdef _USE_DEBUGGING																	
	 ARRAY<DRAWINGRENDERCONTAINER, DEBUG_RI_END>	 m_arrDrawDebugRenderList;
	RENDERDEBUGGING																	m_stRenderDebugging;
#endif
};

END
