#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H

#include "ACoreObject.h"

BEGIN(Core)
class AAnimator;
class APawn;
class ABoneNode;
class ACollider;
class AAnimation;

class CORE_DLL AAnimController abstract : public ACoreObject {
public:
	AAnimController(OBJCON_CONSTRUCTOR, SHPTR<APawn> _spPawn, 
		const _string& _strFolderPath, const _string& _strFileName, const _float4x4& _PivotMatrix = _float4x4::Identity);
	DESTRUCTOR(AAnimController)
public:
	virtual void Tick(const _double& _dTimeDelta) PURE;

	// Get BoneNode
	SHPTR<ABoneNode> FindBoneNode(const _string& _strBoneNode) const;
	// Update 
	void UpdateAttackData(const _bool _isCanAttackSituation, CSHPTRREF<ACollider> _spCollider);
	// AnimEvent Reset
	void ResetCurAnimEvent();
	// Set Animation
	void SetAnimation(const _int& _iAnimIndex);
	void SetAnimation(const _string& _strAnimName);
	void SetAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc);
	void SetAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc);
	// Change Animation
	void ChangeAnimation(const _int& _iAnimIndex);
	void ChangeAnimation(const _string& _strAnimName);
	// Change Animation, Apply Next Time Acc 
	void ChangeAnimation(const _int& _iAnimIndex, const _double& _dNextTimeAcc);
	void ChangeAnimation(const _string& _strAnimName, const _double& _dNextTimeAcc);

	SHPTR<ACollider> BringAttackCollider(_int _iColliderType);
	SHPTR<ACollider> GetAttackCollider();
	_bool IsCollisionAttackCollider(SHPTR<ACollider> _spEnemyCollider);
	void UpdateState(const _string& _strTrigger, const _int _AnimState);
	void ClearState();
public: /* get set */
	const _int GetCurAnimIndex() const;
	SHPTR<AAnimation> GetCurAnimation() const;
	SHPTR<AAnimation> GetNextAnimation() const;
	SHPTR<APawn> GetOwner() const { return m_wpOwnerPawn.lock(); }
	const _int GetAnimState() const { return m_iAnimState; }

	SHPTR<AAnimator> GetAnimator() { return m_spAnimator; }
	const _string& GetInputTrigger() const { READ_SPINLOCK(m_TriggerLock); return m_strInputTrigger; }

	AFastSpinLock& GetChangeOwnerDataLock() { return m_ChangeOwnerDataLock; }
protected:
	void SetSupplyLerpValue(const _float _fSupplyLerpValue);
	void SetAnimState(_int _iAnimState);
	void SetInputTrigger(const _string& _inputTrigger) { WRITE_SPINLOCK(m_TriggerLock);  this->m_strInputTrigger = _inputTrigger; }
private:
	virtual void Free() override;
private:
	_string							m_strInputTrigger;
	AFastSpinLock				m_TriggerLock;
	// Animator, OwenrPawn
	SHPTR<AAnimator>	m_spAnimator;
	WKPTR<APawn>			m_wpOwnerPawn;
	// Anim State
	ATOMIC<_int>				m_iAnimState;

	// Owner data를 바꾸기 위한 함수
	AFastSpinLock				m_ChangeOwnerDataLock;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_AANIMCONTROLLER_H
