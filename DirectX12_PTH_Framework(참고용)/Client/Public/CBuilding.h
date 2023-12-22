#ifndef _PTH_FRAMEWORK_CLIENT_PUBLIC_CBUILDING_H
#define _PTH_FRAMEWORK_CLIENT_PUBLIC_CBUILDING_H

#include "UPawn.h"

BEGIN(Engine)
class UModel;
class UCollider;
END

BEGIN(Client)
class CPlayer;

class CBuilding final : public UPawn {
public:
	CBuilding(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	CBuilding(const CBuilding& _rhs);
	DESTRUCTOR(CBuilding)
public:
	CLONE_MACRO(CBuilding, "CBuilding::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _vecDatas) override;
	bool Hit(CSHPTRREF<CPlayer> _pEnemy);
protected:
	// Tick, LateTick, Render
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	// Damaged
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy);
private:
	SHPTR<UModel>		m_spModel;

	SHPTR< UCollider>	m_spCollider;
	SHPTR<UCollider>	m_spFirstCheckCollider;
};

END

#endif // _PTH_FRAMEWORK_CLIENT_PUBLIC_CBUILDING_H