#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H

#include "AMonster.h"

BEGIN(Server)
class CMummy;

class CSarcophagus final : public AMonster {
public:
	CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType, SHPTR<AJobTimer> _spMonsterJobTimer);
	DESTRUCTOR(CSarcophagus)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
public: /* get set*/
	void SetMummy(SHPTR<CMummy> _spMummy) { this->m_spMummy = _spMummy; }

	SHPTR<CMummy> GetMummy() const { return m_spMummy; }
protected:
	// 영구적으로 해당 오브젝트를 사용하지 않도록 결정할 경우 보낼 메시지 
	virtual void LastBehavior() override;
private:
	virtual void Free() override;
private:
	SARCOPHAGUSTYPE		m_eSarcophagusType;
	SHPTR<CMummy>			m_spMummy;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H