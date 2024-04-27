#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UModel.h"
#include "UTransform.h"
#include "UShaderConstantBuffer.h"
#include "UShader.h"
#include "UModelMaterial.h"
#include "CRooms.h"
#include "CMap.h"
#include "UStageManager.h"

CMap::CMap(CSHPTRREF<UDevice> _spDevice) : UComponent(_spDevice),
m_spRoomContainer{}
{
}

CMap::CMap(const CMap& _rhs) : UComponent(_rhs),
m_spRoomContainer{}
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

	m_spStageManager = Create<UStageManager>();
	m_spRoomContainer = Create<ROOMCONTAINER>();

	return S_OK;
}

HRESULT CMap::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	return E_NOTIMPL;
}

void CMap::LoadRooms()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
	SHPTR<FILEGROUP> MapFolder = spGameInstance->FindFolder(L"Map");

	SHPTR<FILEGROUP> ConvertFolder = MapFolder->FindGroup(L"Convert");
	if (nullptr != ConvertFolder && 0 <= ConvertFolder->FileDataList.size())
	{
		for (const FILEPAIR& File : ConvertFolder->FileDataList)
		{
			_wstring ModelProtoTag = L"Proto_Res_Model_";
			_wstring FileName = File.second->wstrfileName;
			size_t pos = FileName.find(L"_FBX.bin");
			if (pos != _wstring::npos)
				FileName.erase(pos, FileName.length());
			ModelProtoTag.append(FileName);

			CRooms::ROOMDESC tDesc;
			tDesc._wsRoomName = FileName;

			SHPTR<CRooms> _Room = std::static_pointer_cast<CRooms>(spGameInstance->CloneActorAdd(PROTO_ACTOR_ROOM, { &tDesc }));
			_Room->SetModel(ModelProtoTag);

			m_spRoomContainer->emplace(FileName, _Room);

			pos = ModelProtoTag.find(FileName);
			if (pos != _wstring::npos)
				ModelProtoTag.erase(pos, FileName.length());
		}
	}
	else
	{
		return;
	}
}

_bool CMap::LoadNavigation()
{
	return m_spStageManager->Load();
}
