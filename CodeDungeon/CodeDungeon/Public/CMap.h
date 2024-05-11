#pragma once
#include "UComponent.h"
#include "UMapLayout.h"

BEGIN(ENGINE)
class UStageManager;
class UMapLayout;
END

/*
@ Date: 2024-04-23, Writer: 이성현
@ Explain
- 맵을 취급하는 클래스
*/
BEGIN(Client)
class CRooms;
class CModelObjects;

using ROOMCONTAINER = UNORMAP<_wstring, SHPTR<CRooms>>;
using STATICOBJCONTAINER = std::multimap<_wstring, SHPTR<CModelObjects>>;
class CMap : public UComponent
{
public:
	CMap(CSHPTRREF <UDevice> _spDevice);
	CMap(const CMap& _rhs);
	DESTRUCTOR(CMap)

	// UComponent을(를) 통해 상속됨
	void Free() override;
	SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) override;
	HRESULT NativeConstruct() override;
	HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override;

	void LoadRooms();
	_bool LoadNavigation();
	void LoadStaticObjects();
	
	CSHPTRREF<UStageManager> GetStageManager() { return m_spStageManager;}
	CSHPTRREF<ROOMCONTAINER> GetRooms() { return m_spRoomContainer; }
	CSHPTRREF<STATICOBJCONTAINER> GetStaticObjs() { return m_spStaticObjContainer;}

	void AddLightCount() { m_iLightCount++; }
	const _uint GetLightCount() { return m_iLightCount; }
private:
	SHPTR<ROOMCONTAINER>			m_spRoomContainer;
	SHPTR<UStageManager>			m_spStageManager;
	SHPTR<UMapLayout>				m_spMapLayout;
	SHPTR<STATICOBJCONTAINER>		m_spStaticObjContainer;
	_uint							m_iLightCount;

};
END
