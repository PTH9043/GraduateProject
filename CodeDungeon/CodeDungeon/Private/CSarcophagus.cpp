#include "ClientDefines.h"
#include "CMob.h"
#include "CSarcophagus.h"
#include "UGameInstance.h"
#include "URenderer.h"
#include "CMainCamera.h"
#include "UTransform.h"
#include "UAnimModel.h"
#include "CMummyAnimController.h"
#include "UMethod.h"

CSarcophagus::CSarcophagus(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType)
	: CMob(_spDevice, _wstrLayer, _eCloneType), m_SarcophagusType{}
{
}

CSarcophagus::CSarcophagus(const CSarcophagus& _rhs)
	: CMob(_rhs), m_SarcophagusType{}
{
}

void CSarcophagus::Free()
{
}

HRESULT CSarcophagus::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT CSarcophagus::NativeConstructClone(const VOIDDATAS& _Datas)
{
	RETURN_CHECK_FAILED(__super::NativeConstructClone(_Datas), E_FAIL);

	return S_OK;
}

void CSarcophagus::TickActive(const _double& _dTimeDelta)
{
	__super::TickActive(_dTimeDelta);
	static _double elapsedTime = 0;
	_double SarcophagusOpeningSpeed = 20;
	_double SarcophagusTimeArcOpenStart = 50;
	_double SarcophagusTimeArcOpenEnd = 50;

	GetAnimationController()->Tick(_dTimeDelta);

	if(GetFoundTargetState())
	{
		elapsedTime += _dTimeDelta * SarcophagusOpeningSpeed;
	}
	if (elapsedTime < SarcophagusTimeArcOpenEnd)
		GetAnimModel()->TickAnimToTimeAccChangeTransform(GetTransform(), _dTimeDelta, SarcophagusTimeArcOpenStart + elapsedTime);

}

void CSarcophagus::LateTickActive(const _double& _dTimeDelta)
{
	GetRenderer()->AddRenderGroup(RENDERID::RI_NONALPHA_LAST, GetShader(), ThisShared<UPawn>());
}

HRESULT CSarcophagus::RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderActive(_spCommand, _spTableDescriptor);
}

HRESULT CSarcophagus::RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor)
{
	return __super::RenderShadowActive(_spCommand, _spTableDescriptor);
}
HRESULT CSarcophagus::RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass)
{
	return __super::RenderOutlineActive(_spCommand, _spTableDescriptor,_pass);
}

void CSarcophagus::Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta)
{
}
