#include "ServerDefines.h"
#include "CShurikenThrowing.h"
#include "ACoreInstance.h"
#include"ATransform.h"
#include "ACollider.h"

namespace Server
{
	CShurikenThrowing::CShurikenThrowing(OBJCON_CONSTRUCTOR, SESSIONID _ID) : 
		AStaticObject(OBJCON_CONDATA, _ID), m_fTraveledDistance{0}, m_isThrown{false}
	{
	}

	_bool CShurikenThrowing::Start(const VOIDDATAS& _ReceiveDatas)
	{
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		_float4x4 mWorldMatrix = *static_cast<_float4x4*>(_ReceiveDatas[0]);
		GetTransform()->SetNewWorldMtx(mWorldMatrix);

		InsertColliderContainer(COLLIDERTYPE::COLLIDER_MAIN, ACollider::TYPE_OBB,
			COLLIDERDESC{ {1.f, 1.f, 1.f}, {0.f, 0.f, 0.f} });

		//for (auto& iter : GetColliderContainer())
		//{
		//	iter.second->SetScaleToFitModel()
		//}
		return true;
	}

	void CShurikenThrowing::ThrowShurikens(const _double& _dTimeDelta, const Vector3& _vDir)
	{
		RETURN_CHECK(false == m_isThrown, ;);

		SHPTR<ATransform> spTransform = GetTransform();

		const static _float ACTIVE_DISTANCE{ 100 };
		const static _float MOVESPEED{ 250 };
		const static _float ROTSPEED{ DirectX::XMConvertToDegrees(90.f) };

		if (m_fTraveledDistance < ACTIVE_DISTANCE)
		{
			Vector3 vMovement = _vDir * MOVESPEED * _dTimeDelta;
			m_fTraveledDistance += vMovement.Length();
			spTransform->TranslateDir(_vDir, _dTimeDelta, MOVESPEED);
			spTransform->RotateTurn(Vector3::Right, ROTSPEED, _dTimeDelta);
		}
	}

	void CShurikenThrowing::State(SHPTR<ASession> _spSession)
	{
	}

	void CShurikenThrowing::Collision(AGameObject* _pGameObject, const _double& _dTimeDelta)
	{
	}

	void CShurikenThrowing::LastBehavior()
	{
	}

	void CShurikenThrowing::Free()
	{
	}

}
