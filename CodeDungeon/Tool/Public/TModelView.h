#pragma once 
#include "TImGuiView.h"

BEGIN(Engine)
class UModel;
class UAnimModel;
END

BEGIN(Tool)
class TShowModelObject;
class TShowAnimModelObject;
/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- Model들을 로드해서 보여주기 위한 클래스
*/
class TModelView final : public TImGuiView {
public:
	TModelView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TModelView)
	DESTRUCTOR(TModelView)
public:
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

	void ShowModels();
	void ShowAnimMoldels();
	// Show Model List
	void ShowModelList();
	void ShowAnimModelList();

	void ConvertModels();
	void ConvertAnimModels();

	void LoadAssimpModelDatas(CSHPTRREF<FILEGROUP> _spFolder);
	void LoadAssimpAnimModelDatas(CSHPTRREF<FILEGROUP> _spFolder);
	void ResetModels();
	void ResetAnimModels();
private:
	using MODELS = UNORMAP<_string, SHPTR<UModel>>;
	using ANIMMODEL = UNORMAP<_string, SHPTR<UAnimModel>>;
	using FILEGROUPS = UNORMAP<_string, SHPTR<FILEGROUP>>;
	using FOLDERPAIR = std::pair<_wstring, SHPTR<FILEGROUP>>;
	using FILEPAIR = std::pair<_wstring, SHPTR<FILEDATA>>;

	MAINDESC												m_stMainDesc;
	DOCKDESC											m_stModelDockDesc;
	DOCKDESC											m_stAnimModelDockDesc;

	MODELS													m_ModelsContainer;
	ANIMMODEL											m_AnimModelContainer;

	SHPTR<FILEGROUP>							m_spModelFileFolder;
	SHPTR<FILEGROUP>							m_spAnimModelFileFolder;

	SHPTR<TShowAnimModelObject>	m_spShowAnimModelObject;
	SHPTR<TShowModelObject>			m_spShowModelObject;

	_uint														m_iFileIndex;

	_bool														m_isAllConverter;
	_float3													m_vModelScale;
	_float3													m_vAnimModelScale;

	_bool														m_isInitSetting;
	_bool														m_isResetModel;
	_bool														m_isResetAnimModel;

	_float3													m_vModelPivotScale;
	_float3													m_vAnimModelPivotScale;
};

END
