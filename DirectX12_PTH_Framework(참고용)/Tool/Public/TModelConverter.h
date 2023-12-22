#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TMODELCONVERTER_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TMODELCONVERTER_H

#include "TImGuiView.h"

BEGIN(Engine)
class UModel;
class UAnimModel;
END

BEGIN(Tool)
class TShowModelObject;
class TShowAnimModelObject;

class TModelConverter : public TImGuiView{
public:
	TModelConverter(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TModelConverter)
	DESTRUCTOR(TModelConverter)
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
private:
	using MODELS = UNORMAP<_string, SHPTR<UModel>>;
	using ANIMMODEL = UNORMAP<_string, SHPTR<UAnimModel>>;
	using FILEGROUPS = UNORMAP<_string, SHPTR<FILEGROUP>>;
	using FOLDERPAIR = std::pair<_wstring, SHPTR<FILEGROUP>>;
	using FILEPAIR = std::pair<_wstring, SHPTR<FILEDATA>>;

	MAINDESC												m_stMainDesc;
	DOCKDESC											m_stModelDockDesc;
	DOCKDESC											m_stAnimModelDockDesc;

	MODELS													m_conunomapModels;
	ANIMMODEL											m_conunomapAnimModel;

	SHPTR<FILEGROUP>							m_spModelFileFolder;
	SHPTR<FILEGROUP>							m_spAnimModelFileFolder;

	SHPTR<TShowAnimModelObject>	m_spShowAnimModelObject;
	SHPTR<TShowModelObject>			m_spShowModelObject;

	_uint														m_iFileIndex;

	_bool														m_isAllConverter;
	_float3													m_vModelScale;
	_float3													m_vAnimModelScale;

	_bool														m_isInitSetting;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TMODELCONVERTER_H