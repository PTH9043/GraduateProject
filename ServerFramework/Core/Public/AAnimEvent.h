#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AANIMEVENT_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AANIMEVENT_H

#include "ACoreBase.h"

//BEGIN(Core)
//class AGameObject;
//
//
//class AAnimEvent abstract : public ACoreBase {
//protected:
//	AAnimEvent();
//	AAnimEvent(const AAnimEvent& _rhs);
//	DESTRUCTOR(AAnimEvent)
//public:
//	const ANIMEVENTTYPE GetAnimEventType() const { return m_AnimEventType; }
//	const ANIMEVENTCATEGORY GetAnimEventCategory() const { return m_AnimEventCategory; }
//public:
//	virtual SHPTR<AAnimEvent> Clone(UAnimModel* _pAnimModel = nullptr) PURE;
//public:
//	virtual _bool EventCheck(AGameObject* _pGameObject, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc,
//		const _wstring& _wstrInputTrigger) PURE;
//	// AnimEventDesc를 상속받는 구조체를 리턴하는 함수
//	virtual const  ANIMEVENTDESC* OutAnimEventDesc() PURE;
//	// AnimEvent에 다른 구조체가 필요할 경우 해당 구조체의 상속을 받는 녀석들만 내보낼 수 있도록 한다.
//	virtual const ANIMOTHEREVENTDESC* OutOtherEventDesc() PURE;
//
//	virtual void SaveEvent(std::ofstream& _save) PURE;
//	virtual void LoadEvent(CSHPTRREF<UAnimModel> _spAnimModel, std::ifstream& _load) PURE;
//protected:
//	// Event 상황일 때를 정의
//	virtual void EventSituation(UPawn* _pPawn, UAnimModel* _pAnimModel, const _double& _dTimeDelta, const _double& _dTimeAcc) PURE;
//private:
//	virtual void Free() override PURE;
//};
//
//END

#endif