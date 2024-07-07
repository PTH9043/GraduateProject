#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H

#include "AGameObject.h"

BEGIN(Core)
class AAnimator;
class AAnimController;

class CORE_DLL APawn abstract : public AGameObject {
public:
	APawn(OBJCON_CONSTRUCTOR, SESSIONID _ID, SESSIONTYPE _SessionType);
	DESTRUCTOR(APawn)
public: /* get set */
	SHPTR<AAnimController> GetAnimController() { return m_spAnimController; }
protected: /* get set*/
	void SetAnimController(SHPTR<AAnimController> _spAnimController) { this->m_spAnimController = _spAnimController; }
private:
	virtual void Free() override;
private:
	// Animator
	SHPTR< AAnimController>		m_spAnimController;
};


END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_APAWN_H