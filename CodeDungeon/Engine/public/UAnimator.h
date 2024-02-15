#pragma once

#include "UComponent.h"

BEGIN(Engine)
class UAnimClip;
class UAnimModel;
class URootBoneNode;


using ANIMCLIPCONTAINER = UNORMAP<_wstring, SHPTR<UAnimClip>>;

class UAnimator final : public UComponent {
public:
	struct ANIMATORDESC {
		_wstring	 wstrAnimModelProtoType;

		ANIMATORDESC() : wstrAnimModelProtoType{ L"" } {}
		ANIMATORDESC(const _wstring& _wstrAnimModelProtoTypeName) :
			wstrAnimModelProtoType{ _wstrAnimModelProtoTypeName } {}
	};
public:
	UAnimator(CSHPTRREF<UDevice> _spDevice);
	UAnimator(const UAnimator& _rhs);
	DESTRUCTOR(UAnimator)
public:
	virtual void Free() override;
	CLONE_MACRO(UAnimator, "UAnimator::Clone To Failed")
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;

	void Save(const _wstring& _wstrPath);
	void SavePathIsFolder(const _wstring& _wstrPath);
	void Load(const _wstring& _wstrPath);
	void LoadPathIsFolder(const _wstring& _wstrPath);
private:
	SHPTR<UAnimModel>			m_spAnimModel;
	SHPTR<URootBoneNode>		m_spRootBoneNode;
	ANIMCLIPCONTAINER				m_AnimClipContainer;
};

END

