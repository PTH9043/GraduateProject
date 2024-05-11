#pragma once

#include "UItem.h"


BEGIN(Engine)
class UCharacter;
class UCollider;
class UBoneNode;
class UShaderConstantBuffer;
class UModel;
/*
@ Date: 2024-04-27, Writer: 박태현
@ Explain
-  캐릭터가 들고 있게 하는 장비에 대한 클래스이다. 
*/
class UEquipment : public UItem {
	using HASBUFFERCONTAINER = ARRAY<_int, MAX_HAS_TEX>;
public:
	struct DESC {
		EQUIPTYPE					eEquipType;
		// 주인이 되는 Pawn
		SHPTR<UCharacter>	spOwner;
		SHPTR<UModel>			spModel;
		EQUIPMENTINFO			EquipmentInfo;
		_wstring							wstrBoneNodeName;

		DESC() :eEquipType{EQUIPTYPE::EQUIP_END}, spOwner{nullptr}, spModel{nullptr},
			EquipmentInfo{}, wstrBoneNodeName{L""} { }
 		DESC(EQUIPTYPE _EquipType, CSHPTRREF<UCharacter> _spOwner, CSHPTRREF<UModel> _spModel,
			const EQUIPMENTINFO& _EquipmentInfo, const _wstring& _wstrBoneNodeName) :
			eEquipType{_EquipType}, spOwner{ _spOwner }, spModel{_spModel},
			EquipmentInfo{ _EquipmentInfo }, wstrBoneNodeName{ _wstrBoneNodeName }
		{}

		void Load(const _wstring& _wstrPath);
		void Save(const _wstring& _wstrPath);
	};
	enum ORDER{ FIRST = 0 };
public:
	UEquipment(CSHPTRREF<UDevice> _spDevice, const _wstring& _wstrLayer, const CLONETYPE& _eCloneType);
	UEquipment(const UEquipment& _rhs);
	DESTRUCTOR(UEquipment)
public:
	// UPawn을(를) 통해 상속됨
	CLONE_MACRO(UEquipment, "UEquipment::Clone To Failed")
	virtual void Free() override;
	virtual HRESULT NativeConstruct() override;
	virtual HRESULT NativeConstructClone(const VOIDDATAS& _Datas) override;
	void BindBoneNode(const _wstring& _wstrBoneNode);
public: /* get set */
	const EQUIPMENTINFO& GetEquipmentInfo() const { return m_EquipmentInfo; }
	CSHPTRREF<UModel> GetEquipModel() const { return m_spEquipModel; }
	CSHPTRREF<UBoneNode> GetEquipBoneNode() const { return m_spEquipBoneNode; }

	void SetEquipmentInfo(const EQUIPMENTINFO _EquipmentInfo) { this->m_EquipmentInfo = _EquipmentInfo; }
	void SetEquipModel(CSHPTRREF<UModel> _spModel) { this->m_spEquipModel = _spModel; }
	void SetEquipBoneNode(CSHPTRREF<UBoneNode> _spBoneNode) { this->m_spEquipBoneNode = _spBoneNode; }
protected:
	virtual void TickActive(const _double& _dTimeDelta) override;
	virtual void LateTickActive(const _double& _dTimeDelta) override;
	virtual HRESULT RenderActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual HRESULT RenderShadowActive(CSHPTRREF<UCommand> _spCommand, CSHPTRREF<UTableDescriptor> _spTableDescriptor) override;
	virtual void Collision(CSHPTRREF<UPawn> _pEnemy) override;

	void BindShaderBuffer();
protected: /* get set */
	SHPTR<UCharacter> GetOwner() const { return m_wpOwner.lock();  }
private:
	EQUIPTYPE												m_eEquipType;
	SHPTR<UModel>										m_spEquipModel;
	// Owner Character
	WKPTR<UCharacter>								m_wpOwner;
	EQUIPMENTINFO										m_EquipmentInfo;
	// 장착되는 본 노드
	SHPTR<UBoneNode>								m_spEquipBoneNode;
	SOCKETMATRIXPARAM							m_SockMatrixParam;
	SHPTR< UShaderConstantBuffer>		m_spSocketMatrixBuffer;
	SHPTR<UShaderConstantBuffer>		m_spTexCheckBuffer;
	// MaxHasTex
	HASBUFFERCONTAINER							m_HasTexContainer;
};

END

