#include "ClientDefine.h"
#include "CBuilding.h"
#include "UTexGroup.h"
#include "UMethod.h"
#include "UModel.h"
#include "UTransform.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "CPlayer.h"

CBuilding::CBuilding(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType) : 
	UPawn(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC),
	m_spModel{ nullptr },
	m_spCollider{ nullptr },
	m_spFirstCheckCollider{ nullptr }
{
}

CBuilding::CBuilding(const CBuilding& _rhs) : 
	UPawn(_rhs),
	m_spModel{ nullptr },
	m_spCollider{ nullptr },
	m_spFirstCheckCollider{ nullptr }
{
}

void CBuilding::Free()
{
}

HRESULT CBuilding::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CBuilding::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	if (FAILED(__super::NativeConstructClone(_vecDatas)))
		return E_FAIL;

	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	_wstring PROTO_MODEL{ PROTO_RES_BUILDINGMODEL_01};
	m_spModel = static_pointer_cast<UModel>(spGameInstance->CloneResource(PROTO_MODEL));
	AddShader(PROTO_RES_MODELSHADER, RES_SHADER);
	GetTransform()->SetPos({ 300.f, 0.f, 300.f });

	UCollider::COLLIDERDESC tDesc;
	{
		tDesc.vScale = _float3{ 300.f, 300.f, 300.f };
		tDesc.vTranslation = _float3{ 0.f, 100.f, 0.f };
	}
	m_spCollider = AddComponent<UCollider>(PROTO_COMP_OBBCOLLIDER, L"OBBCollider", { &tDesc });
	{
		tDesc.vScale = _float3{ 300.f, 300.f, 300.f };
		tDesc.vTranslation = _float3{ 0.f, 100.f, 0.f };
	}
	m_spFirstCheckCollider = AddComponent<UCollider>(PROTO_COMP_ABBCOLLIDER, L"ABBCollider", { &tDesc });
	return S_OK;
}

bool CBuilding::Hit(CSHPTRREF<CPlayer> _pEnemy)
{
	RETURN_CHECK(nullptr == _pEnemy, false);
	RETURN_CHECK(nullptr == m_spFirstCheckCollider, false);
	RETURN_CHECK(nullptr == m_spCollider, false);

	if (false == m_spFirstCheckCollider->IsCollision(_pEnemy->GetFirstCheckCollider()))
		return false;

	if (false == m_spCollider->IsCollision(_pEnemy->GetCollider()))
		return false;

	Collision(_pEnemy);
	return true;
}

void CBuilding::TickActive(const _double& _dTimeDelta)
{
	m_spCollider->SetTransform(GetTransform());
	m_spFirstCheckCollider->SetTransform(GetTransform());
}

void CBuilding::LateTickActive(const _double& _dTimeDelta)
{
//	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
//	if (false == spGameInstance->IsFrustomContains(GetTransform()->GetPos(), 20.f))
	//	return;

	if (nullptr != m_spModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}

#ifdef _USE_DEBUGGING
	m_spCollider->AddRenderer(RI_NONALPHA_LAST);
	m_spFirstCheckCollider->AddRenderer(RI_NONALPHA_LAST);
#endif 
}

HRESULT CBuilding::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
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

void CBuilding::Collision(CSHPTRREF<UPawn> _pEnemy)
{

}
