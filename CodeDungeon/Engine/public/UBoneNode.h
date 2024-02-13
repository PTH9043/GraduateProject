#pragma once 
#include "UBase.h"

BEGIN(Engine)
class UModel;
/*
@ Date: 2024-02-04, Writer: 박태현
@ Explain
- 모델의 뼈 정보를 담고 있는 클래스
*/
class  UBoneNode : public UBase{
public:
	UBoneNode();
	UBoneNode(const UBoneNode& _rhs);
	DESTRUCTOR(UBoneNode)
public:
	static void SetNodeBaseName(const _wstring& _wstrNodeBaseName) { BASE_BONENODE_NAME = _wstrNodeBaseName; }
	static _wstring& GetBaseNodeName() { return BASE_BONENODE_NAME; }

	const _float4x4& GetOffsetMatrix() const { return m_mOffsetMatrix; }
	const _float4x4& GetTransformMatrix() const { return m_mTransformMatrix; }
	const _float4x4& GetCombineMatrix() const { return m_mCombineTransformMatirx; }
	const _float GetDepth() const { return m_fDepths; }
	const _wstring& GetName() const { return m_wstrName; }

	void SetTransformMatrix(const _float4x4& _mTranformMatrix) { this->m_mTransformMatrix = _mTranformMatrix; }
	void SetOffsetmatrix(const _float4x4& _mOffsetMatrix) { this->m_mOffsetMatrix = _mOffsetMatrix; }
public:
	SHPTR<UBoneNode> Clone();
	virtual void Free() override;
	HRESULT NativeConstruct(const BONENODEDESC& _stBoneNodeDesc);
	void FindParents(CSHPTRREF<UModel> _spModel);
	virtual void UpdateCombinedMatrix();
protected:
	const _float3& GetCombineMatrixLook() const { return *((_float3*)&m_mCombineTransformMatirx.m[DirectX::PTH::MATROW_LOOK][0]); }
	const _float3& GetCombineMatrixPos() const { return *((_float3*)&m_mCombineTransformMatirx.m[DirectX::PTH::MATROW_POS][0]); }
protected:
	void RemoveCombineMatrixData();
	void ComputeCombinedMatrix();
private:
	// 뼈의 가장 기본이 되는 것의 이름 (중요한 것)
	static	_wstring					BASE_BONENODE_NAME;

	_float4x4							m_mOffsetMatrix;
	_float4x4							m_mTransformMatrix;
	_float4x4							m_mCombineTransformMatirx;
	//ParentsNode
	SHPTR<UBoneNode>		m_spParentsNode;
	_wstring								m_wstrName{};
	_wstring								m_wstrParetnsName{};
	_float									m_fDepths;
};

END


