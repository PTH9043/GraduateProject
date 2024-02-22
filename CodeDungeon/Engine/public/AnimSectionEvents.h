#pragma once
#include "AnimEventParents.h"

BEGIN(Engine)
class UCollider;
class UBoneNode;

/*
@ Date: 2024-02-17, Writer: 박태현
@ Explain
- 구간 사이에서 특정 이벤트가 발생할 때 다음 애니메이션으로 넘어가게 만드는 이벤트이다. 
*/
class UAnimChangeBetweenEvent final : public UAnimSectionEvent{
public:
	UAnimChangeBetweenEvent();
	UAnimChangeBetweenEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimChangeBetweenEvent)
public:
	// UAnimSectionEvent을(를) 통해 상속됨
	virtual const ANIMOTHEREVENTDESC*  OutOtherEventDesc() override;
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	// 애니메이션과 애니메이션 사이를 변경하는 이벤트
	ANIMCHANGEDESC		m_AnimChangeDesc;
};

/*
=================================================
AnimChangeBetweenEvent
=================================================
AnimColliderEvent
=================================================
*/

/*
@ Date: 2024-02-22, Writer: 박태현
@ Explain
- 구간 사이에서 특정 이벤트가 발생할 때 다음 애니메이션으로 넘어가게 만드는 이벤트이다.
*/
class UAnimColliderEvent final : public UAnimSectionEvent {
public:
	UAnimColliderEvent();
	UAnimColliderEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load);
	DESTRUCTOR(UAnimColliderEvent)
public:
	virtual _bool EventCheck(UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
		const _wstring& _wstrInputTrigger) override;
	// UAnimSectionEvent을(를) 통해 상속됨
	virtual const  ANIMOTHEREVENTDESC*  OutOtherEventDesc() override;
protected:
	// Event 상황일 때를 정의
	virtual void EventSituation(UAnimModel* _pAnimModel, const _double& _dTimeDelta) override;
	virtual void SaveEvent(std::ofstream& _save) override;
	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) override;
private:
	virtual void Free() override;
private:
	ANIMCOLLIDERDESC	m_AnimColliderDesc;
};


END