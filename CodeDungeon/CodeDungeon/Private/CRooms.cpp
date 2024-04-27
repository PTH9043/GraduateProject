#include "ClientDefines.h"
#include "CRooms.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"

CRooms::CRooms(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
	, m_wsRoomTag{}
{
}

CRooms::CRooms(const CRooms& _rhs) :
	CModelObjects(_rhs)
	, m_wsRoomTag{}
{
}

void CRooms::Free()
{
}

HRESULT CRooms::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CRooms::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);
	ROOMDESC tDesc = UMethod::ConvertTemplate_Index<ROOMDESC>(_vecDatas, 0);
	m_wsRoomTag = tDesc._wsRoomName;

	return S_OK;
}

void CRooms::Collision(CSHPTRREF<UPawn> _pEnemy)
{
	
}
