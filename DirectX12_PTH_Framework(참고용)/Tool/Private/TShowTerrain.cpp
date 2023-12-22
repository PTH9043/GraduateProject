#include "ToolDefines.h"
#include "TShowTerrain.h"
#include "UVIBufferTerrain.h"
#include "UTexGroup.h"
#include "UTransform.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "UMethod.h"

TShowTerrain::TShowTerrain(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::STATIC),
	m_spVIBufferTerrain{ nullptr }, m_spTextureGroup{ nullptr }, m_wstrDiffuseTexName{ L"" }
{
}

TShowTerrain::TShowTerrain(const TShowTerrain& _rhs) : UPawn(_rhs),
m_spVIBufferTerrain{ _rhs.m_spVIBufferTerrain }, m_spTextureGroup{ _rhs.m_spTextureGroup }
, m_wstrDiffuseTexName{ _rhs.m_wstrDiffuseTexName }
{
}

void TShowTerrain::Free()
{
}

HRESULT TShowTerrain::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	return S_OK;
}

HRESULT TShowTerrain::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	AddShader(PROTO_RES_NORMALOBJECTSHADER);
	return S_OK;
}

void TShowTerrain::TerrainHeightPos(_float3& _vPosition)
{
	RETURN_CHECK(nullptr == m_spVIBufferTerrain, ;);
	m_spVIBufferTerrain->TerrainHeightPos(_vPosition, GetTransform()->GetWorldMatrix());
}

void TShowTerrain::TickActive(const _double& _dTimeDelta)
{
}

void TShowTerrain::LateTickActive(const _double& _dTimeDelta)
{
	RETURN_CHECK(nullptr == m_spVIBufferTerrain, ;);
	AddRenderGroup(RENDERID::RI_NONALPHA_MIDDLE);
}

HRESULT TShowTerrain::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	if (nullptr != m_spTextureGroup)
	{
		m_spTextureGroup->SetUpTextureName(GetShader(), SRV_REGISTER::T0, m_wstrDiffuseTexName);
	}
	{
		GetTransform()->BindTransformData(GetShader());
	}
	m_spVIBufferTerrain->Render(GetShader(), _spCommand);
	return S_OK;
}

void TShowTerrain::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
