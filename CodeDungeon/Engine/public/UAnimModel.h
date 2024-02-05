#pragma once
#include "UModel.h"

BEGIN(Engine)
class UShaderConstantBuffer;
class UAnimation;
class UTexture;
/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- 모델의 메쉬, 뼈 정보, 모델의 텍스쳐 정보들을 VECTOR 컨테이너들로 담고 있는 클래스이다.
애니메이션 정보도 들고 있어서 애니메이션을 실행할 수 있다.
*/
class  UAnimModel : public UModel{
	using ANIMATIONS = VECTOR<SHPTR<UAnimation>>;
	using SETUPBONEMATRIXES = ARRAY<_float4x4, MAX_BONE_SIZE>;
	using ANIMSTRINGS = UNORMAP<_wstring, _uint>;
public:
	UAnimModel(CSHPTRREF<UDevice> _spDevice);
	UAnimModel(const UAnimModel& _rhs);
	DESTRUCTOR(UAnimModel)
public:
	const _float GetSupplyLerpValue() const { return m_fSupplyLerpValue; }
	ANIMATIONS& GetAnimations() { return m_vecAnimations; }
	const ANIMATIONPARAM& GetAnimParam() const { return m_stAnimParam; }

	void SetSupplyLerpValue(const _float _fSupplyLerpValue) { this->m_fSupplyLerpValue = _fSupplyLerpValue; }
	void SetAnimParam(const ANIMATIONPARAM& _stAnimParam) { this->m_stAnimParam = _stAnimParam; }
public:
	CLONE_MACRO(UAnimModel, "UAnimModel::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	using UModel::NativeConstruct;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	// 애니메이션을 Tick하는 함수
	void TickAnimation(const _double& _dDeltaTime);
	// 애니메이션을 렌더하는 함수
	virtual HRESULT Render(const _uint _iMeshIndex, CSHPTRREF<UShader> _spShader, CSHPTRREF<UCommand> _spCommand) override;
	// Set Animation
	void SetAnimation(const _uint& _iAnimIndex);
	void SetAnimation(const _wstring& _wstrAnimName);
	// Change Animation
	void ChangeAnimation(const _uint& _iAnimIndex);
	void ChangeAnimation(const _wstring& _wstrAnimName);
private:
	// CreateAnimation
	HRESULT CreateAnimation(const VECTOR<ANIMDESC>& _convecAnimDesc, const _wstring& _wstrPath);
	// Load To Data
	virtual void LoadToData(const _wstring& _wstrPath) override;
	void LoadAnimMeshData(std::ifstream& _ifRead, VECTOR<ANIMMESHDESC>& _convecMeshes);
	void LoadAnimationData(std::ifstream& _ifRead, VECTOR<ANIMDESC>& _convecAnimes);

	void ChangeAnimIndex(const _uint& _iAnimIndex, _uint& _iIndex);
	void ChangeAnimIndex(const _wstring& _wstrAnimName, _uint& _iIndex);
	void SettingCurAnimSituation();
	void SettingNextAnimSituation();
	HRESULT CreateShaderConstantBuffer();
private:
	static	const  _wstring											SECTION_FOLDEDER_NAME;

	ANIMATIONS															m_vecAnimations;
	ANIMSTRINGS														m_AnimNamesGroup;
	VECTOR<SETUPBONEMATRIXES>			m_vecSetupBonMatrix;
	// Animations
	SHPTR< UShaderConstantBuffer>					m_spAnimShaderConstantBuffer;
	ANIMATIONPARAM												m_stAnimParam;
	// Bone, PrevBone
	SHPTR<UShaderConstantBuffer>					m_spBoneMatrixShaderConstantBuffer;
	SHPTR<UShaderConstantBuffer>					m_spPrevBoneMatrixShaderConstantBuffer;
	//UpLoadResource
	ComPtr<Dx12Resource>									m_cpUpLoadResource;

	SHPTR<UAnimation>											m_spCurAnimation;
	SHPTR<UAnimation>											m_spNextAnimation;

	_uint																		m_iCurAnimIndex;
	_uint																		m_iNextAnimIndex;
	_float																		m_fSupplyLerpValue;
	_bool																		m_isChangeAnim;
};

END
