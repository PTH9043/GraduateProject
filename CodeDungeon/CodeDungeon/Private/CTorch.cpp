#include "ClientDefines.h"
#include "UGameInstance.h"
#include "UCollider.h"
#include "UTransform.h"
#include "UMethod.h"
#include "CTorch.h"
#include "UFire.h"
#include "UPawn.h"

CTorch::CTorch(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CModelObjects(_spDevice, _wstrLayer, _eCloneType),
	m_spFire{nullptr},
	m_stFireNoiseBuffer{},
	m_stFireDistortionBuffer{}
{
}

CTorch::CTorch(const CTorch& _rhs)
	: CModelObjects(_rhs),
	m_spFire{ nullptr },
	m_stFireNoiseBuffer{},
	m_stFireDistortionBuffer{}
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
	SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);

	SetModel(L"Proto_Res_Model_Torch_FBX.bin");

	TORCHDESC tTorchDesc = UMethod::ConvertTemplate_Index<TORCHDESC>(_vecDatas, 0);
	GetTransform()->SetNewWorldMtx(tTorchDesc._Worldm);

	UFire::FIREDESC tFireDesc;
	tFireDesc.wstrFireShader = PROTO_RES_2DFIRESHADER;
	m_spFire = std::static_pointer_cast<UFire>(spGameInstance->CloneActorAdd(PROTO_ACTOR_FIRE, { &tFireDesc }));
	m_spFire->SetActive(true);
	static float ScaleX = 3.f;
	static float ScaleY = 6.65f;
	_float3 ScaleFloat3 = _float3(ScaleX, ScaleY, 1);

	m_spFire->GetTransform()->SetScale(ScaleFloat3);
	m_spFire->GetTransform()->SetPos(_float3(GetTransform()->GetPos().x, GetTransform()->GetPos().y + 7.f, GetTransform()->GetPos().z));

	m_stFireNoiseBuffer = m_spFire->GetFireNoiseBuffer();
	m_stFireDistortionBuffer = m_spFire->GetFireDistortionBuffer();


	return S_OK;
}
