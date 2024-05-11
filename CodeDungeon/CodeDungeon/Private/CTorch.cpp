#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CTorch.h"

CTorch::CTorch(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType)
{
}

CTorch::CTorch(const CTorch& _rhs)
	: CModelObjects(_rhs)
{
}

void CTorch::Free()
{
}

HRESULT CTorch::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CTorch::NativeConstructClone(const VOIDDATAS& _vecDatas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_vecDatas), E_FAIL);

	SetModel(L"Proto_Res_Model_Torch_FBX.bin");

	return S_OK;
}
