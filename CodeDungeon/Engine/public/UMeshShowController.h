#pragma once
#include "UController.h"


BEGIN(Engine)
class UMeshContainer;

/*
@ Date: 2024-05-21, Writer: 박태현
@ Explain
-   메쉬의 보여줌을 관장하는 컨트롤러
*/
class UMeshShowController : public UController{
	struct MESHSHOWINFO
	{
		_bool isMeshShowEnable;
		_int	   iMeshGroupIndex;

		MESHSHOWINFO() : isMeshShowEnable{ false }, iMeshGroupIndex{ 0 } {}
	};
public:
	using OWNERMESHCONTAINER = UNORMAP<_uint, SHPTR< UMeshContainer>>;
	using OWNERSHOWCONTAINER = VECTOR<MESHSHOWINFO>;

	struct DESC{
		VECTOR<SHPTR<UMeshContainer>> MeshContainer;
	};
public:
	UMeshShowController(CSHPTRREF<UDevice> _spDevice);
	UMeshShowController(const UMeshShowController& _rhs);
	virtual ~UMeshShowController() = default;
public:
	CLONE_MACRO(UMeshShowController, "UMeshShowController::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	HRESULT NativeConstruct(const _wstring& _wstrPath);
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;
	// Tick
	virtual void Tick(const _double& _dTimeDelta);

	_bool IsShowEnable(const _int _iMeshIndex);
public:
	void Load(const _wstring& _wstrPath);
	void Save(const _wstring& _wstrPath);
private:
	virtual void Free() override;
private:
	OWNERMESHCONTAINER			m_OwnerMeshContainer;
	OWNERSHOWCONTAINER			m_OwnerShowContainer;
};


END
