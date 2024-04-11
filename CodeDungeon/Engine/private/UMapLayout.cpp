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

void UMapLayout::AddtoMapContainer(const _string& _ObjName, TFDESC& _ObjData)
{
	m_spMapObjectsContainer->emplace(_ObjName, _ObjData);
}

void UMapLayout::ConfigModelTFData(CSHPTRREF<UPawn> _Pawn, TFDESC* _TFData, MapObjectType _Type)
{
	_TFData->eType = _Type;
	_TFData->vPos = _Pawn->GetTransform()->GetPos();
	_TFData->vRotate = _Pawn->GetTransform()->GetRotation();
	_TFData->vScale = _Pawn->GetTransform()->GetScale();
}

_bool UMapLayout::Save(const _wstring& _wstrPath)
{
	std::ofstream save{ _wstrPath, std::ios::binary };
	RETURN_CHECK(save.fail(), false);

	_uint mapSize = static_cast<_uint>(m_spMapObjectsContainer->size());
	save.write(reinterpret_cast<const char*>(&mapSize), sizeof(_uint));

	for (const auto& pair : (*m_spMapObjectsContainer)) {
		size_t keySize = pair.first.size();
		save.write(reinterpret_cast<const char*>(&keySize), sizeof(size_t));
		save.write(pair.first.c_str(), keySize);

		// Write the value (TFDESC)
		save.write(reinterpret_cast<const char*>(&pair.second), sizeof(TFDESC));
	}
	save.close();
	return true;
}


_bool UMapLayout::Load(const _wstring& _wstrPath)
{
	return _bool();
}

