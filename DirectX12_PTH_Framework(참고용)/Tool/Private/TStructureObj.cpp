#include "ToolDefines.h"
#include "TStructureObj.h"
#include "UModel.h"
#include "UTransform.h"
#include "UShader.h"

TStructureObj::TStructureObj(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType),
m_spModel{ nullptr }
{
}

TStructureObj::TStructureObj(const TStructureObj& _rhs) : 
	UPawn(_rhs),
	m_spModel{ nullptr }
{
}

void TStructureObj::Free()
{
}

HRESULT TStructureObj::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TStructureObj::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);
	return S_OK;
}

void TStructureObj::TickActive(const _double& _dTimeDelta)
{
}

void TStructureObj::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT TStructureObj::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescripto)
{
	if (nullptr != m_spModel)
	{
		__super::RenderActive(_spCommand, _spTableDescripto);

		for (_uint i = 0; i < m_spModel->GetMeshContainerCnt(); ++i)
		{
			// Bind Transform 
			GetTransform()->BindTransformData(GetShader());

			m_spModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
			m_spModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_NORMALS, GetShader());
			// Render
			m_spModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

void TStructureObj::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
