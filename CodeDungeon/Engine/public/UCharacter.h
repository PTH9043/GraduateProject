#pragma once

#include "UPawn.h"

BEGIN(Engine)
class UTransform;
/*
@ Date: 2024-02-25, Writer: 박태현
@ Explain
-  사용자가 움직이거나 AI가 움직일 때 사용하는 클래스이다. 
*/
class UCharacter abstract : public UPawn {
public:
	UCharacter(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UCharacter(const UCharacter& _rhs);
	DESTRUCTOR(UCharacter)
public:
	// UPawn을(를) 통해 상속됨
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _vecDatas) PURE;
	virtual void Free() PURE;
	virtual HRESULT NativeConstruct() PURE;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) PURE;
public:
	// 다른 캐릭터와의 거리 산출 
	_float OtherCharacterToDistance(CSHPTRREF<UActor> _spOtherActor);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UActor> _spOtherActor);
	// 다른 캐릭터를 기준으로 바라보는 자기 자신의 Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UActor> _spOtherActor);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UActor>  _spOtherActor);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UActor>  _spOtherActor);
	// 다른 캐릭터와의 거리 산출 
	_float OtherCharacterToDistance(CSHPTRREF<UTransform> _spOtherTransform);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Angle 
	_float OtherCharacterDirToLook(CSHPTRREF<UTransform> _spOtherTransform);
	// 다른 캐릭터를 기준으로 바라보는 자기 자신의 Right Angle 
	_float OhterCharacterDirToRight(CSHPTRREF<UTransform> _spOtherTransform);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Angle ( 180 ~ -180 )
	_float OtherCharacterDirToLookConverter(CSHPTRREF<UTransform> _spOtherTransform);
	// 다른 캐릭터를 바라보는 자기 자신의 Look Direction
	_float3 OtherCharacterDirToLookVectorF3(CSHPTRREF<UTransform> _spOtherTransform);
protected:
	virtual void TickActive(const _double& _dTimeDelta) PURE;
	virtual void LateTickActive(const _double& _dTimeDelta) PURE;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) PURE;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) PURE;
private:

};

END
