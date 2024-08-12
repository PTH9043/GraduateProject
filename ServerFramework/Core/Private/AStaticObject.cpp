#include "CoreDefines.h"
#include "AStaticObject.h"
#include "ASession.h"
#include "ATransform.h"

namespace Core {

	AStaticObject::AStaticObject(OBJCON_CONSTRUCTOR, SESSIONID _ID) : 
		AGameObject(OBJCON_CONDATA, _ID, SESSIONTYPE::OBJECT), m_fActiveRange{ 0 }, m_isCurrentFindPlayer{ false }
	{
		SetActive(true);
	}

	_bool AStaticObject::Start(const VOIDDATAS& _ReceiveDatas)
	{
		return __super::Start(_ReceiveDatas);
	}

	void AStaticObject::FindPlayer(SHPTR<ASession> _spSession)
	{
		RETURN_CHECK(nullptr == _spSession, ;);
		SHPTR<ATransform> spMobTr = GetTransform();
		SHPTR<ATransform> spPlayerTr = _spSession->GetTransform();
		{
			Vector3 vMobPos = spMobTr->GetPos();
			Vector3 vPlayerPos = spPlayerTr->GetPos();
			Vector3 vDirection = vPlayerPos - vMobPos;

			float fDistanceToPlayer = vDirection.Length();
			fDistanceToPlayer = vDirection.Length();

			if (fDistanceToPlayer <= m_fActiveRange)
			{
				m_isCurrentFindPlayer = true;
			}
			else
			{
				m_isCurrentFindPlayer = false;
			}
		}
	}

	void AStaticObject::Free()
	{
	}


}