#pragma once
#include "ToolDefines.h"
#include "Scene.h"
class ToolGridObject;

class ToolScene : public CScene
{
public:
	virtual void BuildObjects(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, const ComPtr<ID3D12RootSignature>& _RootSignature);
	virtual void ReleaseObjects();
	virtual void Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera);

	shared_ptr<ToolGridObject> m_pToolGrid;
};

