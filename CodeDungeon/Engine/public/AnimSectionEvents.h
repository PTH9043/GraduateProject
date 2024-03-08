#pragma once
#include "UAnimEvent.h"

BEGIN(Engine)

/*
@ Date: 2024-02-21, Writer: 박태현
@ Explain
- 구간에만 이벤트를 발생시키는 클래스
*/
class UAnimSectionEvent final : public UAnimEvent {
public:
	UAnimSectionEvent();
	UAnimSectionEvent(std::ifstream& _load);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	virtual ANIMEVENTDESC* const OutAnimEventDesc() override { return &m_AnimSectionDesc; }
	virtual void ChangeAnimEventDesc(ANIMEVENTDESC* _AnimEventDesc) override;
	virtual ANIMOTHEREVENTDESC* const OutOtherEventDesc() override;
	virtual void ChangeOtherEventDesc(ANIMOTHEREVENTDESC* _AnimOtherEventDesc) override;
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMEVENTSECTIONDESC			m_AnimSectionDesc;
	// 애니메이션과 애니메이션 사이를 변경하는 이벤트
	ANIMCHANGEDESC						m_AnimChangeDesc;
};

END
