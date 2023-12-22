#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWTERRAIN_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWTERRAIN_H

#include "UPawn.h"

BEGIN(Engine)
class UVIBufferTerrain;
class UTexGroup;
class UNavigation;

END

BEGIN(Tool)

class DEF_CACHE_ALGIN TShowTerrain : public UPawn {
public:
	typedef struct tagTerrainDesc {
		_wstring				wstrBufferProtoName{};
		_wstring				wstrTextureProtoName{};
		_wstring				wstrDiffuseTexName{ L"" };
	}TERRAINDESC;
public:
	TShowTerrain(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	TShowTerrain(const TShowTerrain& _rhs);
	DESTRUCTOR(TShowTerrain)
public:
	CSHPTRREF<UVIBufferTerrain> GetVIBufferTerrain() const { return m_spVIBufferTerrain; }
	CSHPTRREF<UTexGroup> GetTextureGroup() const { return m_spTextureGroup; }
	const _wstring& GetTextureName() const { return m_wstrDiffuseTexName; }

	void SetTerrrainBuffer(CSHPTRREF<UVIBufferTerrain> _spTerrainBuffer) { this->m_spVIBufferTerrain = _spTerrainBuffer; }
	void SetTerrainTexGroup(CSHPTRREF<UTexGroup> _spTerrainTexGroup) { this->m_spTextureGroup = _spTerrainTexGroup; }
	void SetTextureName(const _wstring& _wstrTextureName) { this->m_wstrDiffuseTexName = _wstrTextureName; }
public:
	CLONE_MACRO(TShowTerrain, "TShowTerrain::Clone To Failed")
	virtual void Free() override;
	// Native Construct
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;

	void TerrainHeightPos(_float3& _vPosition);
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
private:
	SHPTR<UVIBufferTerrain>	m_spVIBufferTerrain;
	SHPTR<UTexGroup>				m_spTextureGroup;
	 _wstring									m_wstrDiffuseTexName;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TSHOWTERRAIN_H