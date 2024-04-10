#include "ToolDefines.h"
#include "TShowModelObject.h"
#include "UModel.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"

TShowModelObject::TShowModelObject(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: UPawn(_spDevice, _wstrLayer, _eCloneType),
	m_spModel{ nullptr }
{
}

TShowModelObject::TShowModelObject(const TShowModelObject& _rhs) :
	UPawn(_rhs),
	m_spModel{ nullptr }
{
}

void TShowModelObject::Free()
{
}

HRESULT TShowModelObject::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT TShowModelObject::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	m_spShaderNormalCheckBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::MODELCHECKBUF, sizeof(int));

	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);
	return S_OK;
}

void TShowModelObject::TickActive(const _double& _dTimeDelta)
{

}

void TShowModelObject::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT TShowModelObject::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
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
			
			if (m_spModel->GetMaterials()[0]->arrMaterialTexture[TEXTYPE::TextureType_NORMALS] == nullptr) {
				HasNormalTex = 0;
				GetShader()->BindCBVBuffer(m_spShaderNormalCheckBuffer, &HasNormalTex, sizeof(int));
			}
			else {
				HasNormalTex = 1;
				GetShader()->BindCBVBuffer(m_spShaderNormalCheckBuffer, &HasNormalTex, sizeof(int));
			}
			// Render
			m_spModel->Render(i, GetShader(), _spCommand);
		}
	}
	return S_OK;
}

void TShowModelObject::Collision(CSHPTRREF<UPawn> _pEnemy)
{

}
