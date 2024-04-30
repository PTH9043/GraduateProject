#pragma once
#include "UController.h"

BEGIN(Engine)
/*
@ Date: 2024-04-28, Writer: 박태현
@ Explain
-  사용자의 입력을 받아서 움직이는 클래스
*/
class UPlayerController abstract : public UController  {
public:
	// 들어온 인풋의 종류
	enum class INPUTTYPE { DOWN, UP, PRESSING, INPUTTYPE_END };
public:
	struct INPUTDESC
	{
		INPUTTYPE		eInputType;
		_ubyte				ubInputKey;

		INPUTDESC() : eInputType{ INPUTTYPE::INPUTTYPE_END}, ubInputKey{0}{}
		// 주소비교
		_bool operator==(const INPUTDESC& _inputDesc) { return this == &_inputDesc; }
		_bool operator!=(const INPUTDESC& _inputDesc) { return this != &_inputDesc; }
	};
public:
	UPlayerController(CSHPTRREF<UDevice> _spDevice, const _int _iKeyLength);
	UPlayerController(const UPlayerController& _rhs);
	virtual ~UPlayerController() = default;
public:
	virtual void Free() override PURE;
	virtual SHPTR<UCloneObject> Clone(const VOIDDATAS& _tDatas) PURE;
public:
	// Native Construct 
	virtual HRESULT NativeConstruct() override  PURE;
	// Clone 
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _tDatas) override  PURE;
	// Tick
	virtual void Tick(UCharacter* _pCharacter, const _double& _dTimeDelta) override  PURE;
	// InputTriggerData
	virtual _bool InputTriggerData(UCharacter* _pCharacter, const _int _Data) override  PURE;
protected:
	// CheckInput
	virtual void CheckInput(const INPUTDESC _InputDesc) PURE;
private:
	VECTOR<_bool>			m_PressingKeyContainer;
	VECTOR<_bool>			m_DownKeyContainer;
	VECTOR<_bool>			m_UpKeyContainer;
	// KeyLength
	_int									m_iKeyLength;
	// Desc
	SET<INPUTDESC>		m_InputContainer;
};

END