#pragma once
#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UChannel;
/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- Animation를 담는 클래스 Channel들의 VECTOR 컨테이너를 들고 있다. 
*/
class   UAnimation : public UBase{
public:
	UAnimation();
	UAnimation(const UAnimation& _rhs);
	DESTRUCTOR(UAnimation)
public:
	const _wstring& GetAnimName() const { return m_wstrName; }
	const _bool IsFinished() const { return m_isFinished; }
	const _bool IsStop() const { return m_isStop; }
	const _bool IsRepeat() const { return m_isRepeat; }
	const _bool IsSupplySituation() const { return m_isSupplySituation; }
	const _double& GetDuration() const { return m_dDuration; }
	const _double& GetTimeAcc() const { return m_dTimeAcc; }
	const _float GetTotalAnimFastValue() const { return m_fTotalAnimationFastValue; }

	const VECTOR<ANIMFASTSECTION>& GetAnimFastSection() { return m_AnimFastSections; }

	// Set Finish
	void SetFinish(const _bool _isFinish) { this->m_isFinished = _isFinish; }
	void SetStop(const _bool _isStop) { this->m_isStop = _isStop; }
	void SetRepeat(const _bool _isRepeat) { this->m_isRepeat = _isRepeat; }
	void SetSupplySituation(const _bool _isSupplySituation) { this->m_isSupplySituation = _isSupplySituation; }
	void SetAnimTimeAcc(const _double& _dTimeAcc) { this->m_dTimeAcc = _dTimeAcc; }

	void UpdateAnimFastSections(const _float _fTotalAnimFastValue, const VECTOR<ANIMFASTSECTION>& _AnimFastSection);
public:
	SHPTR<UAnimation> Clone(CSHPTRREF<UAnimModel> _spAnimModel);
	virtual void Free() override;
	HRESULT NativeConstruct(CSHPTRREF<UAnimModel> _spAnimModel, const ANIMDESC& _stAnimDesc);
	void UpdateTransformMatrices(const _double& _dTimeDelta);
	void UpdateTransformMatricesToTimeAcc(const _double& _TimeAcc);
	void UpdateNextAnimTransformMatrices(const _double& _dTimeDelta, const _float _fSupplyValue,
		CSHPTRREF<UAnimation> _spAnimation);

	void ResetData();
	// Save Sections
	void SaveSections(const _wstring& _wstrPath);
	void LoadSections(const _wstring& _wstrPath);
private:
	using CHANNELS = VECTOR<SHPTR<UChannel>>;
	using ANIMFASTSECTIONS = VECTOR<ANIMFASTSECTION>;

	static constexpr _float	MAX_SUPPLY_VALUE{1.f};

	CHANNELS						m_Channels;
	_wstring								m_wstrName;
	_uint									m_iNumChannels;
	_double								m_dTickPerSeconds;
	_double								m_dDuration;
	_double								m_dTimeAcc;
	_bool									m_isFinished;
	_bool									m_isStop;
	_bool									m_isRepeat;
	_bool									m_isSupplySituation;

	_float									m_fSupplySituationValue;

	ANIMFASTSECTIONS		m_AnimFastSections;
	_float									m_fTotalAnimationFastValue;
};

END