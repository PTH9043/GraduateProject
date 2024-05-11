#include "EngineDefine.h"
#include "UEquipment.h"
#include "UMethod.h"
#include "UShaderConstantBuffer.h"
#include "UModel.h"
#include "UTransform.h"
#include "UModelMaterial.h"
#include "UShader.h"
#include "UCharacter.h"
#include "UAnimModel.h"
#include "URootBoneNode.h"

void UEquipment::DESC::Load(const _wstring& _wstrPath)
{
	std::ifstream read{ _wstrPath, std::ios_base::binary };
	read.read((_char*)&eEquipType, sizeof(EQUIPTYPE));
	read.read((_char*)&EquipmentInfo, sizeof(EQUIPMENTINFO));
	UMethod::ReadString(read, wstrBoneNodeName);
}

void UEquipment::DESC::Save(const _wstring& _wstrPath)
{
	std::ofstream write{ _wstrPath, std::ios_base::binary };
	write.write((_char*)&eEquipType, sizeof(EQUIPTYPE));
	write.write((_char*)&EquipmentInfo, sizeof(EQUIPMENTINFO));
	UMethod::SaveString(write, wstrBoneNodeName);
}

/*
=========================================
UEquipment::Struct  DESC
=========================================
UEquipment::Class
=========================================
*/

UEquipment::UEquipment(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, 
	const CLONETYPE& _eCloneType) : 
	UItem(_spDevice, _wstrLayer, _eCloneType, BACKINGTYPE::DYNAMIC, ITEMTYPE::ITEM_EQUIP),
	m_spEquipModel{nullptr},
	m_wpOwner{},
	m_EquipmentInfo{},
	m_spEquipBoneNode{ nullptr },
	m_SockMatrixParam{ _float4x4::Identity },
	m_spSocketMatrixBuffer{ nullptr },
	m_spTexCheckBuffer{nullptr},
	m_HasTexContainer{}
{
}

UEquipment::UEquipment(const UEquipment& _rhs) : 
	UItem(_rhs),
	m_spEquipModel{ nullptr },
	m_wpOwner{},
	m_EquipmentInfo{},
	m_spEquipBoneNode{ nullptr },
	m_SockMatrixParam{ _float4x4::Identity },
	m_spSocketMatrixBuffer{ nullptr },
	m_spTexCheckBuffer{ nullptr },
	m_HasTexContainer{}
{
}


void UEquipment::Free()
{
}

HRESULT UEquipment::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UEquipment::NativeConstructClone(const VOIDDATAS& _Datas)
{
	if (FAILED(__super::NativeConstructClone(_Datas)))
		return E_FAIL;

	assert(_Datas.size() > 1);
	UEquipment::DESC desc = UMethod::ConvertTemplate_Index<UEquipment::DESC>(_Datas, FIRST);
	m_wpOwner = desc.spOwner;
	m_spEquipModel = desc.spModel;
	::memcpy(&m_EquipmentInfo, &desc.EquipmentInfo, sizeof(EQUIPMENTINFO));

	SHPTR<UCharacter> spCharacter = m_wpOwner.lock();
	// Find Root Bone Node 
	if (nullptr != spCharacter)
	{
		m_spEquipBoneNode = spCharacter->GetAnimModel()->FindBoneNode(desc.wstrBoneNodeName);
	}
	if (nullptr != m_spEquipModel)
	{
		_uint ModelBufferSize = m_spEquipModel->GetMeshContainerCnt();

		m_spSocketMatrixBuffer = CreateNative< UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::SOCKETMATRIX, GetTypeSize<SOCKETMATRIXPARAM>(), ModelBufferSize);
		m_spTexCheckBuffer = CreateNative<UShaderConstantBuffer>(GetDevice(), CBV_REGISTER::MODELCHECKBUF, GetTypeSize< HASBUFFERCONTAINER>(), ModelBufferSize);
	}
	return S_OK;
}

void UEquipment::BindBoneNode(const _wstring& _wstrBoneNode)
{
	SHPTR<UCharacter> spCharacter = m_wpOwner.lock();
	m_spEquipBoneNode = spCharacter->GetAnimModel()->FindBoneNode(_wstrBoneNode);
}

void UEquipment::TickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spEquipBoneNode)
	{
		// Get CombineMatrix
		m_SockMatrixParam.SocketMatrix = m_spEquipBoneNode->GetCombineMatrix();
	}
}

void UEquipment::LateTickActive(const _double& _dTimeDelta)
{
	if (nullptr != m_spEquipModel)
	{
		AddRenderGroup(RI_NONALPHA_MIDDLE);
	}
}

HRESULT UEquipment::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	__super::RenderActive(_spCommand, _spTableDescriptor);

	for (_uint i = 0; i < m_spEquipModel->GetMeshContainerCnt(); ++i)
	{
		// Bind Transform 
		GetTransform()->BindTransformData(GetShader());

		m_spEquipModel->BindTexture(i, SRV_REGISTER::T0, TEXTYPE::TextureType_DIFFUSE, GetShader());
		m_spEquipModel->BindTexture(i, SRV_REGISTER::T1, TEXTYPE::TextureType_SPECULAR, GetShader());
		m_spEquipModel->BindTexture(i, SRV_REGISTER::T2, TEXTYPE::TextureType_NORMALS, GetShader());

		m_HasTexContainer[0] = m_spEquipModel->GetMaterials()[i]->IsEmpty(TextureType_DIFFUSE) == true ? 0 : 1;
		m_HasTexContainer[1] = m_spEquipModel->GetMaterials()[i]->IsEmpty(TextureType_SPECULAR) == true ? 0 : 1;
		m_HasTexContainer[2] = m_spEquipModel->GetMaterials()[i]->IsEmpty(TextureType_NORMALS) == true ? 0 : 1;

		BindShaderBuffer();
		// Render
		m_spEquipModel->Render(i, GetShader(), _spCommand);
	}
	return S_OK;
}

HRESULT UEquipment::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return S_OK;
}

void UEquipment::Collision(CSHPTRREF<UPawn> _pEnemy)
{
}

void UEquipment::BindShaderBuffer()
{
	GetShader()->BindCBVBuffer(m_spSocketMatrixBuffer, &m_SockMatrixParam, GetTypeSize<SOCKETMATRIXPARAM>());
	GetShader()->BindCBVBuffer(m_spTexCheckBuffer, &m_HasTexContainer, GetTypeSize< HASBUFFERCONTAINER>());
}
