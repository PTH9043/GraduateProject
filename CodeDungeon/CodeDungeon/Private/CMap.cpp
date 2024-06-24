#include "ClientDefines.h"
#include "UGameInstance.h"
#include "CMap.h"
#include "UCollider.h"
#include "UModel.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UModelMaterial.h"
#include "CRooms.h"
#include "UStageManager.h"
#include "UMapLayout.h"
#include "CTorch.h"
#include "CIronBars.h"
#include "CModelObjects.h"



CMap::CMap(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice),
m_spRoomContainer{},
m_spMapLayout{ nullptr },
m_iLightCount{ 0 }
{
}

CMap::CMap(const CMap& _rhs) : UComponent(_rhs),
m_spRoomContainer{},
m_spMapLayout{ nullptr },
m_iLightCount{0}
{
}

void CMap::Free()
{
	m_spRoomContainer.reset();
}

SHPTR<UCloneObject> CMap::Clone(const VOIDDATAS& _tDatas)
{
	return SHPTR<UCloneObject>();
}

HRESULT CMap::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	m_spRoomContainer = Create<ROOMCONTAINER>();
	m_spMapLayout = CreateConstructorNativeNotMsg<UMapLayout>(spGameInstance->GetDevice());
	m_spStaticObjContainer = Create<STATICOBJCONTAINER>();

	return S_OK;
}

HRESULT CMap::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	return S_OK;
}

void CMap::LoadRooms()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<FILEGROUP> MapFolder = spGameInstance->FindFolder(L"Map");

	for (const FOLDERPAIR& Folder : MapFolder->UnderFileGroupList)
	{
		SHPTR<FILEGROUP> ConvertFolder = Folder.second->FindGroup(L"Convert");
		if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
		{
			for (const FILEPAIR& File : ConvertFolder->FileDataList)
			{
				_wstring FileName = File.second->wstrfileName;

				CRooms::ROOMDESC tDesc;
				tDesc._wsRoomName = FileName;

				SHPTR<CRooms> _Room = std::static_pointer_cast<CRooms>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ROOM, { &tDesc }));
				m_spRoomContainer->emplace(FileName, _Room);
			}
		}
		else
		{
			return;
		}
	}
}

void CMap::LoadStaticObjects()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	m_spMapLayout->Load();
	OBJCONTAINER _TorchVec;
	OBJCONTAINER _BarsVec;
	for (auto& it : (*m_spMapLayout->GetMapObjectsContainer().get()))
	{
		for (auto& vecit : it.second)
		{
			if(vecit._sModelName == "Torch_FBX.bin")
			{		
				CTorch::TORCHDESC torchDesc;
				torchDesc._Worldm = vecit._mWorldMatrix;
				SHPTR<CTorch> _Torch = std::static_pointer_cast<CTorch>(spGameInstance->CloneActorAdd(PROTO_ACTOR_TORCH, {&torchDesc}));
				_TorchVec.push_back(_Torch);		
			}
			if (vecit._sModelName == "Bars_FBX.bin")
			{
				CIronBars::IRONBARSDESC BarsDesc;
				BarsDesc._Worldm = vecit._mWorldMatrix;
				SHPTR<CIronBars> _IronBars = std::static_pointer_cast<CIronBars>(spGameInstance->CloneActorAdd(PROTO_ACTOR_IRONBARS, { &BarsDesc }));
				_BarsVec.push_back(_IronBars);
				spGameInstance->AddCollisionPawnList(_IronBars);
			}
		}
	}
	m_spStaticObjContainer->emplace("Torch_FBX.bin", _TorchVec);
	m_spStaticObjContainer->emplace("Bars_FBX.bin", _BarsVec);
}
