#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimator;
/*
@ Date: 2024-02-10, Writer: 박태현
@ Explain
- Animation Event를 정의할 부모 클래스
*/
class UAnimEvent abstract : public UBase {
protected:
	UAnimEvent(ANIMEVENTCATEGORY _AnimEventCategory);
	DESTRUCTOR(UAnimEvent)
public:
	const ANIMEVENTCATEGORY GetAnimEventCategory() const { return m_AnimEventCategory; }
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc, 
		const _wstring& _wstrInputTrigger) PURE;
	// AnimEventDesc를 상속받는 구조체를 리턴하는 함수
	virtual ANIMEVENTDESC* const OutAnimEventDesc() PURE;
	// AnimEvent를 상속받는 구조체의 값을 변경하기 위한 함수
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) PURE;
	// AnimEvent에 다른 구조체가 필요할 경우 해당 구조체의 상속을 받는 녀석들만 내보낼 수 있도록 한다.
	virtual ANIMOTHEREVENTDESC* const OutOtherEventDesc() PURE;
	// AnimEvent에 다른 구조체가 필요할 경우 해당 구조체의 상속을 받는 녀석들만 받을 수 있도록 한다. 
	virtual void ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc) PURE;

	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(std::ifstream& _load) PURE;
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTCATEGORY	m_AnimEventCategory;
};

END