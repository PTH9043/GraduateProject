#pragma once 
#include "UBase.h"

BEGIN(Tool)
class TAssimpChannel;
class TAssimpModel;
/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- Assimp를 로드하기 위한 Animation 클래스
*/
class TAssimpAnimation : public UBase {
public:
	TAssimpAnimation();
	TAssimpAnimation(const TAssimpAnimation& _rhs);
	DESTRUCTOR(TAssimpAnimation)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(struct aiAnimation* _pAIAnim, CSHPTRREF< TAssimpModel> _spModel);
	HRESULT NativeConstruct(struct aiAnimation* _pAIAnim, CSHPTRREF< TAssimpModel> _spModel, const _wstring& _strName);
	void UpdateTransformMatrices(const _double& _dTimeDelta);
	// AnimDesc
	void GetData(ANIMDESC& _stAnimDesc);
private:
	void AnimationStringNameSplit(_wstring& _wstrAnimName);
private:
	using CHANNELS = VECTOR<SHPTR<TAssimpChannel>>;
	_wstring				m_wstrName;
	_uint					m_iNumChannels;
	_double				m_dTickPerSeconds;
	_double				m_dDuration;
	CHANNELS		m_vecAssimpChannels;
	_double				m_dTimeAcc;
	_bool					m_isFinished;
};

END
