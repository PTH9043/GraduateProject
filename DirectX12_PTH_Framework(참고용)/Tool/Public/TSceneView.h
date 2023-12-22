#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TSCENEVIEW_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TSCENEVIEW_H

#include "TImGuiView.h"

BEGIN(Engine)
class USkyBox;
class UActor;
class UTexture;
class UTexGroup;
class UTerrain;
class UTexture;
END

BEGIN(Tool)

using GAMEOBJECTLIST = LIST<SHPTR<UActor>>;
using GAMEOBJECTCONTAINERS = UNORMAP<_string, GAMEOBJECTLIST>;
using GAMELAYERCONTAINERS = UNORMAP<_string, GAMEOBJECTCONTAINERS >;

using VIEWMENUS = VECTOR<_bool>;

class TShowModelObject;
class TStructureObj;
class TShowTerrain;

class TSceneView : public TImGuiView{
public:
	enum OPENVIEW{
		VIEW_TERRAIN, VIEW_SKYBOX, VIEW_END
	};
public:
	TSceneView(CSHPTRREF<UDevice> _spDevice);
	NO_COPY(TSceneView)
	DESTRUCTOR(TSceneView)
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
	void ShowHieracky();
	void ShowPerspective();
	void ShowTerrainView();
	/// DockBuilding
	void DockBuilding();
	// Update
	void UpdateContainers();
	// Object Counts
	bool IsObjectCountChange();
private:
	MAINDESC														m_stSceneView;
	DOCKDESC													m_stHierackyView;
	DOCKDESC													m_stPerspeciveView;
	DOCKDESC													m_stSkyBoxView;
	DOCKDESC													m_stTerrainView;
	DOCKDESC													m_stModelView;

	_bool																m_isInitSetting;
	ARRAY<_bool, OPENVIEW::VIEW_END> m_arrViewMenus;

	SHPTR<USkyBox>										m_spSkyBox;
	GAMELAYERCONTAINERS							m_conunomapGameLayerMap;
	_uint																m_iGameObjectCnt{ 0 };

	SHPTR<FILEGROUP>									m_spTerrainFolder;
	SHPTR<FILEGROUP>									m_spTerrainHeightMapFolder;

	// Terrain
	SHPTR<TShowTerrain>								m_spShowTerrain;
	SHPTR<UTexGroup>									m_spTerrainDiffuse;
	// File Data
	SHPTR<FILEDATA>										m_spSelectedTerrainFile;
	_float																m_fHeightMapSize;
	// Show Model Object
	SHPTR< TShowModelObject>					m_spShowModelObject;
	// Object List
	LIST<SHPTR< TStructureObj>>				m_spStructedObjectList;
};

END

#endif  // _PTH_FRAMEWORK_TOOL_PUBLIC_TSCENEVIEW_H

