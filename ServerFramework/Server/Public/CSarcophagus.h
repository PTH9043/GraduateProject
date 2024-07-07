#ifndef _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H
#define _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H

#include "AMonster.h"

BEGIN(Server)
class CMummy;

class CSarcophagus final : public AMonster {
public:
	CSarcophagus(OBJCON_CONSTRUCTOR, SESSIONID _ID, SARCOPHAGUSTYPE _eSarcophagusType);
	DESTRUCTOR(CSarcophagus)
public:
	virtual _bool Start(const VOIDDATAS& _ReceiveDatas = {}) override;
	virtual void State(SHPTR<ASession> _spSession, _int _MonsterState = 0) override;
public: /* get set*/
	void SetMummy(SHPTR<CMummy> _spMummy) { this->m_spMummy = _spMummy; }

	SHPTR<CMummy> GetMummy() const { return m_spMummy; }
private:
	virtual void Free() override;
private:
	SARCOPHAGUSTYPE		m_eSarcophagusType;
	SHPTR<CMummy>			m_spMummy;
};

END

#endif // _SERVERFRAMEWORK_SERVER_PUBLIC_CSARCOPHAGUS_H