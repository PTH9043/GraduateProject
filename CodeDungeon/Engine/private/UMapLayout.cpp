#include "EngineDefine.h"
#include "UGameInstance.h"
#include "UMapLayout.h"
#include "UMethod.h"
#include "UPawn.h"
#include "UTransform.h"

UMapLayout::UMapLayout(CSHPTRREF<UDevice> _spDevice)
	: UComponent(_spDevice), 
	m_spMapObjectsContainer{nullptr}
{
}

UMapLayout::UMapLayout(const UMapLayout& _rhs)
	: UComponent(_rhs),
	m_spMapObjectsContainer{nullptr}
{
}

void UMapLayout::Free()
{
}


HRESULT UMapLayout::NativeConstruct()
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	m_spMapObjectsContainer = Create<MAPOBJECTSCONTAINER>();


	return S_OK;
}

HRESULT UMapLayout::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK(_tDatas.size() <= 0, E_FAIL);
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);
	MAPOBJECTSCONTAINER tDesc = UMethod::ConvertTemplate_Index<MAPOBJECTSCONTAINER>(_tDatas, 0);


	return E_NOTIMPL;
}

void UMapLayout::AddtoMapContainer(const _string& _RoomName, MAPOBJECTS& _ObjDatas)
{
	m_spMapObjectsContainer->emplace(_RoomName, _ObjDatas);
}

_bool UMapLayout::Save(const _wstring& _wstrPath)
{
	for (const auto& pair : (*m_spMapObjectsContainer)) {
		_wstring str;
		str.assign(_wstrPath.begin(), _wstrPath.end());
		str.append(L"\\ObjectLayouts\\");

		str.append(UMethod::ConvertSToW(pair.first));

		std::ofstream save{ str, std::ios::binary };
		RETURN_CHECK(save.fail(), false);

		size_t vectorSize = pair.second.size();
		save.write(reinterpret_cast<const char*>(&vectorSize), sizeof(size_t));
		for (int i = 0 ; i < vectorSize; i++)
		{
			size_t objnameSize = pair.second[i]._sModelName.size();
			save.write(reinterpret_cast<const char*>(&objnameSize), sizeof(size_t));
			save.write(pair.second[i]._sModelName.c_str(), objnameSize);

			save.write(reinterpret_cast<const char*>(&pair.second[i]._mWorldMatrix), sizeof(_float4x4));
		}
		save.close();
	}
	return true;
}


_bool UMapLayout::Load()
{
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SHPTR<FILEGROUP> MapLayoutFolder = spGameInstance->FindFolder(L"ObjectLayouts");
	if (m_spMapObjectsContainer->size() > 0)
		m_spMapObjectsContainer->clear();

	if (nullptr != MapLayoutFolder && 0 <= MapLayoutFolder->FileDataList.size())
	{
		for (const FILEPAIR& File : MapLayoutFolder->FileDataList)
		{
			_wstring filePath = File.second->wstrfilePath;
			std::ifstream load{ filePath, std::ios::binary };
			if (load.fail()) {
				return false;
			}
			_string roomName = UMethod::ConvertWToS(File.first);

			size_t vectorSize;
			load.read(reinterpret_cast<char*>(&vectorSize), sizeof(size_t));
	
			MAPOBJECTS objDatas;

			for (size_t i = 0; i < vectorSize; ++i) {
				size_t objNameSize;
				load.read(reinterpret_cast<char*>(&objNameSize), sizeof(size_t));

				_string objName;
				objName.resize(objNameSize);
				load.read(&objName[0], objNameSize);

				_float4x4 worldMatrix;
				load.read(reinterpret_cast<char*>(&worldMatrix), sizeof(_float4x4));

				OBJDESC obj;
				obj._sModelName = objName;
				obj._mWorldMatrix = worldMatrix;
				objDatas.push_back(obj);
			}

			load.close();
			m_spMapObjectsContainer->emplace(roomName, objDatas);
		}
	}

    return true;
}

