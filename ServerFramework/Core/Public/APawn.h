#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H

#include "AGameObject.h"

BEGIN(Core)
class AAnimator;

class CORE_DLL APawn abstract : public AGameObject {
public:
	APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(APawn)
public:
	virtual void AnimTick(const _double _dTimeDelta);
	void CreateAnimator(const _string& _strFolderPath, const _string& _strFileName);
public: /* get set */
	SHPTR<AAnimator> GetAnimator() { return m_spAnimator; }
	const _string GetAnimTrigger() const { READ_LOCK(m_TriggerLock); return  m_strAnimTrigger; }

	void SetAnimTrigger(const _string _strAnimTrigger);
private:
	virtual void Free() override;
private:
	USE_LOCK								m_TriggerLock;
	// Animation Trigger
	_string									m_strAnimTrigger;
	// Animator
	SHPTR< AAnimator>			m_spAnimator;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H