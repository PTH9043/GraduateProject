#include "pch.h"
#include "Shader.h"

CShader::CShader()
{

}

//그래픽스 파이프라인 상태 객체를 생성한다.
CShader::CShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature,const wstring& shaderFile, const string& vs, const string& ps)
{
	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob,error;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, vs.c_str(), "vs_5_1",
		nCompileFlags, 0, pd3dVertexShaderBlob.GetAddressOf(), &error));
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, ps.c_str(), "ps_5_1",
		nCompileFlags, 0, pd3dPixelShaderBlob.GetAddressOf(), &error));
	if (error)
	{
		OutputDebugStringA(reinterpret_cast<char*>(error->GetBufferPointer()));
	}


	

	/*
	ifstream ifsFile{ shaderFile.c_str(), ios::in | ios::binary };
	vector<BYTE> pByteCode{ istreambuf_iterator<char> {ifsFile}, {} };
	UINT nReadBytes = (UINT)ifsFile.tellg();

	D3D12_SHADER_BYTECODE d3dByteCode;
	if (pd3dVertexShaderBlob) {
		HRESULT hResult = D3DCreateBlob(nReadBytes, pd3dVertexShaderBlob.GetAddressOf());
		memcpy((pd3dVertexShaderBlob)->GetBufferPointer(), pByteCode.data(), nReadBytes);
		d3dByteCode.BytecodeLength = (pd3dVertexShaderBlob)->GetBufferSize();
		d3dByteCode.pShaderBytecode = (pd3dVertexShaderBlob)->GetBufferPointer();
	}
	else {
		d3dByteCode.BytecodeLength = nReadBytes;
		d3dByteCode.pShaderBytecode = pByteCode.data();
	}
	*/


	
	

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));
	
	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}


void CShader::SetPipelineState(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	//파이프라인에 그래픽스 상태 객체를 설정한다. 
	_CommandList->SetPipelineState(m_pd3dPipelineStates.Get());
}
void CShader::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
	SetPipelineState(_CommandList);
}




CDiffusedFilledShader::CDiffusedFilledShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& shaderFile, const string& vs, const string& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob, error;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, vs.c_str(), "vs_5_1",
		nCompileFlags, 0, pd3dVertexShaderBlob.GetAddressOf(), &error));
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, ps.c_str(), "ps_5_1",
		nCompileFlags, 0, pd3dPixelShaderBlob.GetAddressOf(), &error));
	if (error)
	{
		OutputDebugStringA(reinterpret_cast<char*>(error->GetBufferPointer()));
	}



	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}
CDiffusedFilledShader::~CDiffusedFilledShader()
{
}


CDiffusedWireFrameShader::CDiffusedWireFrameShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& shaderFile, const string& vs, const string& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob, error;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, vs.c_str(), "vs_5_1",
		nCompileFlags, 0, pd3dVertexShaderBlob.GetAddressOf(), &error));
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, ps.c_str(), "ps_5_1",
		nCompileFlags, 0, pd3dPixelShaderBlob.GetAddressOf(), &error));
	if (error)
	{
		OutputDebugStringA(reinterpret_cast<char*>(error->GetBufferPointer()));
	}


	CD3DX12_RASTERIZER_DESC RasterizerDesc{ D3D12_DEFAULT };
	RasterizerDesc.FillMode = D3D12_FILL_MODE_WIREFRAME;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = RasterizerDesc;
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}
CDiffusedWireFrameShader::~CDiffusedWireFrameShader()
{
}





CTexturedModelShader::CTexturedModelShader(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12RootSignature>& _RootSignature, const wstring& shaderFile, const string& vs, const string& ps)
{
	m_pd3dInputLayouts.clear();
	m_pd3dInputLayouts =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 2, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 3, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 4, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	ComPtr<ID3DBlob> pd3dVertexShaderBlob, pd3dPixelShaderBlob, error;

	UINT nCompileFlags = 0;
#if defined(_DEBUG)
	nCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, vs.c_str(), "vs_5_1",
		nCompileFlags, 0, pd3dVertexShaderBlob.GetAddressOf(), &error));
	DX::ThrowIfFailed(::D3DCompileFromFile(shaderFile.c_str(), NULL, NULL, ps.c_str(), "ps_5_1",
		nCompileFlags, 0, pd3dPixelShaderBlob.GetAddressOf(), &error));
	if (error)
	{
		OutputDebugStringA(reinterpret_cast<char*>(error->GetBufferPointer()));
	}


	D3D12_GRAPHICS_PIPELINE_STATE_DESC d3dPipelineStateDesc;
	::ZeroMemory(&d3dPipelineStateDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	d3dPipelineStateDesc.pRootSignature = _RootSignature.Get();
	d3dPipelineStateDesc.VS = CD3DX12_SHADER_BYTECODE(pd3dVertexShaderBlob.Get());
	d3dPipelineStateDesc.PS = CD3DX12_SHADER_BYTECODE(pd3dPixelShaderBlob.Get());
	d3dPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	d3dPipelineStateDesc.InputLayout = { m_pd3dInputLayouts.data(), (UINT)m_pd3dInputLayouts.size() };
	d3dPipelineStateDesc.SampleMask = UINT_MAX;
	d3dPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	d3dPipelineStateDesc.NumRenderTargets = 1;
	d3dPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3dPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dPipelineStateDesc.SampleDesc.Count = 1;
	d3dPipelineStateDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	_Device->CreateGraphicsPipelineState(&d3dPipelineStateDesc,
		IID_PPV_ARGS(&m_pd3dPipelineStates));

	if (d3dPipelineStateDesc.InputLayout.pInputElementDescs) delete[]
		d3dPipelineStateDesc.InputLayout.pInputElementDescs;
}
CTexturedModelShader::~CTexturedModelShader()
{
}

