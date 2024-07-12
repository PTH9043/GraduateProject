#pragma once
#include "UPlayer.h"

BEGIN(Engine)
class UGameInstance;
class UParticle;
class UParticleSystem;
class UTrail;
class UBlood;
END

BEGIN(Client)
class CSword;
/*
@ Date: 2024-04-30, Writer: 
@ Explain
*/
class CWarriorPlayer final : public UPlayer {
public:
	CWarriorPlayer(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CWarriorPlayer(const CWarriorPlayer& _rhs);
	DESTRUCTOR(CWarriorPlayer)
public:
	CLONE_MACRO(CWarriorPlayer, "CWarriorPlayer::CloneToFailed")
		virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	virtual void ReceiveNetworkProcessData(const UProcessedData& _ProcessData);
public: /* Get Set */
	SHPTR<UParticle>& GetParticle() { return m_spParticle; }

	void IfAttack(_bool is) { isAttack = is; }

	const _bool& GetOBJCollisionState() const { return m_bisCollisionWithObj; }
	void SetOBJCollisionState(_bool _newState) { m_bisCollisionWithObj = _newState; }

	const _bool& GetKickedState() const { return m_bisKicked; }
	void SetKickedState(_bool _newState) { m_bisKicked = _newState; }

	const _bool& GetRiseState() const { return m_bisRise; }
	void SetRiseState(_bool _newState) { m_bisRise = _newState; }

	const _double& GetWarriorKickedTimeElapsed() { return m_dKickedElapsed; }
	void SetWarriorKickedTimeElapsed(_double _newTime) { m_dKickedElapsed = _newTime; }

protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderOutlineActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor, _bool _pass = true) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy, const _double& _dTimeDelta) override;
#ifdef _ENABLE_PROTOBUFF
	void SendMoveData(CSHPTRREF<UGameInstance> spGameInstance);
#endif
private:
	void TranslateStateMoveAndRunF(CSHPTRREF<UGameInstance> _spGameInstance, const _double& _dTimeDelta, const _float _fSpeed);
private:
	SHPTR<UParticle>										m_spParticle;
	PARTICLEPARAM* m_stParticleParam;
	ComputeParticleType* m_stParticleType;

	SHPTR<CSword>												m_spSword;
	SHPTR<UTrail>												m_spTrail;
	SHPTR<UBlood>												m_spBlood;
	_bool																isAttack;

	_bool m_bisCollisionWithObj;
	_bool m_bisKicked;
	_bool m_bisRise;

	_double											m_dKickedElapsed;
};
END

