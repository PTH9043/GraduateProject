#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimation;
class UChannel;
class UBoneNode;
class UAnimEvent;
class UAnimModel;
class UAnimator;

using ANIMEVENTCONTAINER = UNORMAP<ANIMEVENTTYPE, VECTOR<SHPTR<UAnimEvent>>>;
/*
@ Date: 2024-02-13, Writer: 박태현
@ Explain
- 애니메이션과 애니메이션 이벤트들을 담는 AnimClip 클래스이다. 해당 클래스를 통해 애니메이션의 이벤트를 관리한다. 
*/
class UAnimClip final : public UBase {
public:
	UAnimClip(SHPTR<UAnimation> _spAnimation);
	UAnimClip(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _Read);
	NO_COPY(UAnimClip)
	DESTRUCTOR(UAnimClip)
public:
	virtual void Free() override;
	void Tick(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta);
	void InsertEvent(ANIMEVENTTYPE _AnimEventType, CSHPTRREF<UAnimEvent> _spAnimEvent);
	void RemoveEvent(CSHPTRREF<UAnimEvent> _spAnimEvent);

	void Save(CSHPTRREF<UAnimator> _spAnimator, std::ofstream& _Save);
	void Load(CSHPTRREF<UAnimator> _spAnimator, std::ifstream& _Read);
private:
	ANIMEVENTCONTAINER		m_AnimEventContainer;
	SHPTR<UAnimation>			m_spAnimation;
};

END