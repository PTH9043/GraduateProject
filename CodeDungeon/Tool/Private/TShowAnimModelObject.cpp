#include "ToolDefines.h"
#include "TShowAnimModelObject.h"
#include "UAnimModel.h"
#include "UTransform.h"
#include "UShader.h"
#include "UGameInstance.h"
#include "UCollider.h"

TShowAnimModelObject::TShowAnimModelObject(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) :
	UPawn(_spDevice, _wstrLayer, _eCloneType),
	m_spModel{nullptr}

{
}

TShowAnimModelObject::TShowAnimModelObject(const TShowAnimModelObject& _rhs) :
	UPawn(_rhs),
	m_spModel{ nullptr }
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
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	UCollider::COLLIDERDESC tDesc;
	tDesc.vTranslation = _float3(0.f, 0.f, 0.f);
	tDesc.vScale = _float3(1.f, 1.f, 1.f);
	SHPTR<UCollider> Collider = static_pointer_cast<UCollider>(spGameInstance->CloneComp(PROTO_COMP_OBBCOLLIDER, { &tDesc }));

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.5f, 1.0f);

	_float3 randomFloat(dis(gen), dis(gen), dis(gen));

	Collider->ChangeColliderColor(randomFloat);

	_wstring mainColliderTag = L"Main";
	AddColliderInContainer(mainColliderTag, Collider);
	AddShader(PROTO_RES_ANIMMODELSHADER, RES_SHADER);

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
