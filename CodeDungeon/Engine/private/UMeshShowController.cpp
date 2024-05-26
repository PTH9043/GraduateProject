#include "EngineDefine.h"
#include "UMeshShowController.h"
#include "UMethod.h"
#include "UMeshContainer.h"

UMeshShowController::UMeshShowController(CSHPTRREF<UDevice> _spDevice) : 
	UController(_spDevice)
{
}

UMeshShowController::UMeshShowController(const UMeshShowController& _rhs) : 
	UController(_rhs)
{
}

HRESULT UMeshShowController::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UMeshShowController::NativeConstruct(const _wstring& _wstrPath)
{
	RETURN_CHECK_FAILED(__super::NativeConstruct(), E_FAIL);
	_wstring wstrPath = _wstrPath;
	wstrPath.append(L"\\MeshShow");
	if (0 != _wmkdir(wstrPath))
	{
		Load(wstrPath);
	}
	return S_OK;
}

HRESULT UMeshShowController::NativeConstructClone(const VOIDDATAS& _tDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_tDatas), E_FAIL);
	// Data에 구조체가 존재하지 않으면
	assert(_tDatas.size() >= 1);
	DESC desc = UMethod::ConvertTemplate_Index<DESC>(_tDatas, 0);
	// MeshContainer가 비어있으면 리턴
	assert(desc.MeshContainer.size() >= 1);
	// Material Container에 집어넣기
	for (auto& iter : desc.MeshContainer)
	{
		// 만약 MeshContainer가 비어있다면 리턴
		assert(iter != nullptr);
		m_OwnerMeshContainer.insert(MakePair(iter->GetMaterialIndex(), iter));
	}
	m_OwnerShowContainer.resize(desc.MeshContainer.size());
	return S_OK;
}

void UMeshShowController::Tick(const _double& _dTimeDelta)
{
}

_bool UMeshShowController::IsShowEnable(const _int _iMeshIndex)
{
	const auto& iter = m_OwnerMeshContainer.find(_iMeshIndex);
	RETURN_CHECK(m_OwnerMeshContainer.end() == iter, false);
	return m_OwnerShowContainer[iter->first].isMeshShowEnable;
}

void UMeshShowController::Load(const _wstring& _wstrPath)
{
	std::ifstream read{ _wstrPath, std::ios::binary };
	// 읽기가 실패했을 경우
	assert(!read);

	_int size = 0;
	read.read((_char*)&size, sizeof(_int));
	m_OwnerShowContainer.resize(size);
	for (_int i = 0; i < size; ++i)
	{
		read.read((_char*)&m_OwnerShowContainer[i], sizeof(MESHSHOWINFO));
	}
}

void UMeshShowController::Save(const _wstring& _wstrPath)
{
	std::ofstream save{ _wstrPath, std::ios::binary };
	assert(!save);

	_int size = static_cast<_int>(m_OwnerMeshContainer.size());
	save.write((_char*)&size, sizeof(_int));
	for (_int i = 0; i < size; ++i)
	{
		save.write((_char*)&m_OwnerShowContainer[i], sizeof(MESHSHOWINFO));
	}
}

void UMeshShowController::Free()
{
}
