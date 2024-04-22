#pragma once

#include "UPawn.h"
#include "UParticleSystem.h"
BEGIN(Engine)

class UTexGroup;
class UVIBufferPoint;

class UParticle : public UPawn{
public:
	struct PARTICLEDESC
	{
		_wstring						 wstrParticleShader{ L"" };
		_wstring						 wstrParticleComputeShader{L""};
		_wstring						 wstrParticleTextureName;
		PARTICLEPARAM					ParticleParam;
	};

	struct PARTICLELOADATA
	{
		_wstring wstrParticleLoadPath;
	};
public:
	using TEXTUREINDEXCONTAINER = UNORMAP<_uint, SRV_REGISTER>;
public:
	UParticle(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UParticle(const UParticle& _rhs);
	DESTRUCTOR(UParticle)
public:
	CLONE_MACRO(UParticle, "UParticle::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _convecDatas) override;

protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void BindShaderBuffer();
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;
	// Save Load
	virtual _bool Save(const _wstring& _wstrPath);
	virtual _bool Load(const _wstring& _wstrPath);
public:
	CSHPTRREF<UTexGroup> GetTextureGroup() const { return m_spTexGroup; }
	CSHPTRREF<UParticleSystem> GetParticleSystem() const { return m_spParticleSystem; }
#ifdef _USE_IMGUI
public:
	void SetTexture(const _wstring& TexName);
	void SetTexture(_uint _index);
	virtual void ShowObjectInfo() override;

#endif
protected:
	SHPTR<UTexGroup>				m_spTexGroup;
	SHPTR< UParticleSystem>	m_spParticleSystem;
	SHPTR<UVIBufferPoint>		m_spVIBufferPoint;
	// Texture Indexs
	TEXTUREINDEXCONTAINER	m_TextureIndexContainer;
	CUSTIMER									m_LifeTimer;
	_uint TextureIndex;
};

END

