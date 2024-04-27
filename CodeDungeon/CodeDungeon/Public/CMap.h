#pragma once
#include "UComponent.h"

BEGIN(ENGINE)
class UStageManager;

END
/*
@ Date: 2024-04-23, Writer: 이성현
@ Explain
- 맵을 취급하는 클래스
*/
BEGIN(Client)
class CRooms;
using ROOMCONTAINER = UNORMAP<_wstring, SHPTR<CRooms>>;
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
	
	CSHPTRREF<UStageManager> GetStageManager() { return m_spStageManager;}
private:
	SHPTR<ROOMCONTAINER>			m_spRoomContainer;
	SHPTR<UStageManager>			m_spStageManager;


};
END
