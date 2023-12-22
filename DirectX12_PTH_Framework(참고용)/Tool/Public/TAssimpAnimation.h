#ifndef _PTH_FRAMEWORK_ENGIEN_PUBLIC_TASSIMPANIMATION_H
#define _PTH_FRAMEWORK_ENGIEN_PUBLIC_TASSIMPANIMATION_H

#include "UBase.h"

BEGIN(Tool)
class TAssimpChannel;
class TAssimpModel;

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

#endif // _PTH_FRAMEWORK_ENGIEN_PUBLIC_TASSIMPANIMATION_H