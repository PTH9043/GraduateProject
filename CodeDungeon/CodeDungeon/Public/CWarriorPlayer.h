#pragma once
#include "UPlayer.h"

BEGIN(Engine)
class UGameInstance;
class UParticle;
class UParticleSystem;
class UTrail;
END

BEGIN(Client)
class CSword;
/*
@ Date: 2024-04-30, Writer: 박태현
@ Explain
-  사용자가 직접 조종하는 클래스이다.
*/
class CWarriorPlayer final : public UPlayer {
public:
	CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CWarriorPlayer(const CWarriorPlayer& _rhs);
	DESTRUCTOR(CWarriorPlayer)
public:
	// UPlayer을(를) 통해 상속됨
	CLONE_MACRO(CWarriorPlayer, "CWarriorPlayer::CloneToFailed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	SHPTR<UParticle>& GetParticle() { return m_spParticle; }

	void IfAttack(_bool is) { isAttack = is; }

	const _bool& GetOBJCollisionState() const { return m_bisCollisionWithObj; }
	void SetOBJCollisionState(_bool _newState) { m_bisCollisionWithObj = _newState; }

protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
private:
	void TranslateStateMoveAndRunF(CSHPTRREF<UGameInstance> _spGameInstance, const _double& _dTimeDelta, const _float _fSpeed);
private:

	SHPTR<UParticle>		m_spParticle;
	PARTICLEPARAM* m_stParticleParam;
	ComputeParticleType* m_stParticleType;

	SHPTR<CSword>	m_spSword;
	SHPTR<UTrail>	m_spTrail;
	_bool isAttack;
	_bool m_bisCollisionWithObj;

	_float3 m_f3CollidedNormal;
};

END

