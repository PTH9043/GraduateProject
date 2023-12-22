#ifndef _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPCHANNEL_H
#define _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPCHANNEL_H

#include "UBase.h"

BEGIN(Tool)
class TAssimpModel;
class TAssimpBoneNode;

class TAssimpChannel : public UBase {
public:
	TAssimpChannel();
	TAssimpChannel(const TAssimpChannel& _rhs);
	DESTRUCTOR(TAssimpChannel)
public:
	virtual void Free() override;
	HRESULT NativeConstruct(struct aiNodeAnim* _pChannel, CSHPTRREF< TAssimpModel> _pModel);
	void GetData(CHANNELDESC& _stChannelDesc);
private:
	using KEYFRAMES = VECTOR<KEYFRAME>;
	KEYFRAMES									m_vecKeyFrames;
	SHPTR<TAssimpBoneNode>		m_spBoneNode;
	_wstring											m_wstrName;
	_uint												m_iNumMaxKeyFrames;
	_uint												m_iCurKeyFrames;
};

END

#endif // _PTH_FRAMEWORK_TOOL_PUBLIC_TASSIMPCHANNEL_H

