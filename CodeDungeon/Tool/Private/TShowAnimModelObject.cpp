#include "ToolDefines.h"
#include "TShowAnimModelObject.h"
#include "UAnimModel.h"
#include "UTransform.h"
#include "UShader.h"

TShowAnimModelObject::TShowAnimModelObject(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType)
{
}

TShowAnimModelObject::TShowAnimModelObject(const TShowAnimModelObject& _rhs) :
	UPawn(_rhs)
{
}

void TShowAnimModelObject::Free()
{
}

HRESULT TShowAnimModelObject::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TShowAnimModelObject::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	AddShader(PROTO_RES_ANIMMODELSHADER, RES_SHADER);
	GetTransform()->SetScale({ 0.05f, 0.05f, 0.05f });

	return S_OK;
}

void TShowAnimModelObject::TickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
		m_spModel->TickAnimation(_dTimeDelta);
}

void TShowAnimModelObject::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT TShowAnimModelObject::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	if (nullptr != m_spModel)
	{
		__super::RenderActive(_spCommand, _spTableDescriptor);

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

void TShowAnimModelObject::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}
