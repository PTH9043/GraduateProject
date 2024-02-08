#pragma once

#include "UBase.h"

BEGIN(Engine)
class UAnimModel;
class UAnimator;
/*
@ Date: 2024-02-08, Writer: 박태현
@ Explain
- Animation Event를 정의할 부모 클래스
*/
class UAnimEvent abstract : public UBase {
public:
	UAnimEvent();
	UAnimEvent(const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimEvent)
public:
	const _int GetEventID() const { return m_EventID; }
	const ANIMEVENT GetAnimEventType() const { return m_AnimEventType; }
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;

	void Save(const _wstring& _wstrPath);
	void Load(const _wstring& _wstrPath);
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent( std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	_int										m_EventID;
	ANIMEVENT						m_AnimEventType;
};

/* 
=====================================
AnimEvent
=====================================
AnimSectionEvent
=====================================
*/

/*
@ Date: 2024-02-08, Writer: 박태현
@ Explain
- 구간에만 이벤트를 발생시키는 클래스
*/
class UAnimSectionEvent abstract : public UAnimEvent {
public:
	UAnimSectionEvent();
	UAnimSectionEvent(const ANIMEVENTSECTIONDESC& _AnimEventDesc, const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimSectionEvent)
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) override;

protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent( std::ofstream& _save) PURE;
	virtual void LoadEvent( std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMEVENTSECTIONDESC			m_AnimEventDesc;
};


/*
=====================================
AnimSectionEvent
=====================================
AnimOccurEvent
=====================================
*/
/*
@ Date: 2024-02-08, Writer: 박태현
@ Explain
- 구간을 지나면 이벤트를 발생시키는 클래스
*/
class UAnimOccurEvent abstract : public UAnimEvent {
public:
	UAnimOccurEvent();
	UAnimOccurEvent(const ANIMOCURRESDESC& _AnimEventDesc, const _int _EventID, ANIMEVENT _AnimEvent);
	DESTRUCTOR(UAnimOccurEvent)
public:
	virtual HRESULT NativeConstruct() PURE;
	virtual _bool EventCheck(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta, const _double& _dTimeAcc) override;
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(CSHPTRREF<UAnimator> _spAnimator, const _double& _dTimeDelta) PURE;
	virtual void SaveEvent(std::ofstream& _save) PURE;
	virtual void LoadEvent(std::ifstream& _load) PURE;
private:
	virtual void Free() PURE;
private:
	ANIMOCURRESDESC		m_AnimOccurDesc;
};

END