#include "EngineDefine.h"
#include "UAnimator.h"
#include "UAnimModel.h"
#include "URootBoneNode.h"
#include "UMethod.h"
#include "UGameInstance.h"

UAnimator::UAnimator(CSHPTRREF<UDevice> _spDevice) :
	UComponent(_spDevice), 
	m_spAnimModel{nullptr}, 
	m_spRootBoneNode{nullptr},
	m_AnimClipContainer{}
{
}

UAnimator::UAnimator(const UAnimator& _rhs) : 
	UComponent(_rhs), 
	m_spAnimModel{ nullptr },
	m_spRootBoneNode{ nullptr },
	m_AnimClipContainer{}
{
}

void UAnimator::Free()
{
}

HRESULT UAnimator::NativeConstruct()
{
	return __super::NativeConstruct();
}

HRESULT UAnimator::NativeConstructClone(const VOIDDATAS& _Datas)
{
	if (_Datas.size() > 0)
	{
		ANIMATORDESC AnimatorDesc = UMethod::ConvertTemplate_Index<ANIMATORDESC>(_Datas, 0);
		ASSERT_CRASH(false == AnimatorDesc.wstrAnimModelProtoType.empty());

		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		m_spAnimModel = std::static_pointer_cast<UAnimModel>(spGameInstance->CloneResource(AnimatorDesc.wstrAnimModelProtoType));
	}
	return S_OK;
}

void UAnimator::Save(const _wstring& _wstrPath)
{
	std::ofstream Save{ _wstrPath, std::ios::binary };

}

void UAnimator::SavePathIsFolder(const _wstring& _wstrPath)
{
	_wstring str = _wstrPath;
	str.append(L"\\AnimData\\");
	if (0 != _wmkdir(str))
	{

	}
}

void UAnimator::Load(const _wstring& _wstrPath)
{
	std::ifstream Load{ _wstrPath, std::ios::binary };
}

void UAnimator::LoadPathIsFolder(const _wstring& _wstrPath)
{
}
