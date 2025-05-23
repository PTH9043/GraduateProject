#pragma once


class CGameObject;
class CMaterial;
class CScene;
class CShader;
#define RESOURCE_TEXTURE1D			0x01
#define RESOURCE_TEXTURE2D			0x02
#define RESOURCE_TEXTURE2D_ARRAY	0x03	//[]
#define RESOURCE_TEXTURE2DARRAY		0x04
#define RESOURCE_TEXTURE_CUBE		0x05
#define RESOURCE_BUFFER				0x06
#define RESOURCE_STRUCTURED_BUFFER	0x07

class CTexture
{
public:
	CTexture(int nTextureResources, UINT nResourceType, int nSamplers, int nRootParameters);
	virtual ~CTexture();

	UINT							m_nTextureType;
private:
	int								m_nReferences = 0;
	int								m_nTextures = 0;

	vector<ComPtr<ID3D12Resource>> m_ppd3dTextures;	// Texture 리소스
	vector<ComPtr<ID3D12Resource>> m_ppd3dTextureUploadBuffers;	//Texture 업로드 버퍼

	vector<UINT> m_pnResourceTypes;		// 각각의 Texture 들의 타입
	//vector<wstring> m_ppstrTextureNames;	// Texture의 이름
	_TCHAR(*m_ppstrTextureNames)[64] = NULL;
	vector<DXGI_FORMAT> m_pdxgiBufferFormats;	// Texture Dxgi 포맷
	vector<int> m_pnBufferElements;
	vector<int> m_pnBufferStrides;
	int m_nRootParameters = 0;		// 루트파라미터 번호
	vector<int> m_pnRootParameterIndices;		// 루트파라미터 인덱스
	vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_pd3dSrvGpuDescriptorHandles;
	vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_pd3dSrvCpuDescriptorHandles;


	int								m_nSamplers = 0;
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_pd3dSamplerGpuDescriptorHandles;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
//----------------------
	void SetSampler(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSamplerGpuDescriptorHandle);

	void UpdateShaderVariable(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, int nParameterIndex, int nTextureIndex);
	void UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList);

	void ReleaseShaderVariables();

	void LoadTextureFromDDSFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, wchar_t* pszFileName, UINT nResourceType, UINT nIndex);
	
	ComPtr<ID3D12Resource> CreateTexture(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT nIndex, UINT nResourceType, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue);
	ComPtr<ID3D12Resource> CreateTexture(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT nWidth, UINT nHeight, UINT nElements, UINT nMipLevels, DXGI_FORMAT dxgiFormat, D3D12_RESOURCE_FLAGS d3dResourceFlags, D3D12_RESOURCE_STATES d3dResourceStates, D3D12_CLEAR_VALUE* pd3dClearValue, UINT nResourceType, UINT nIndex);
	int LoadTextureFromFile(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CGameObject* pParent, FILE* pInFile, shared_ptr<CShader> pShader, UINT nIndex);


	void SetRootParameterIndex(int nIndex, UINT nRootParameterIndex);
	void SetGpuDescriptorHandle(int nIndex, D3D12_GPU_DESCRIPTOR_HANDLE d3dSrvGpuDescriptorHandle);

//---------------------
	int GetRootParameters() { return(m_nRootParameters); }
	int GetTextures() { return(m_nTextures); }
	_TCHAR* GetTextureName(int nIndex) { return(m_ppstrTextureNames[nIndex]); }

	ComPtr<ID3D12Resource> GetResource(int nIndex) { return(m_ppd3dTextures[nIndex]); }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(int nIndex) { return(m_pd3dSrvGpuDescriptorHandles[nIndex]); }
	int GetRootParameter(int nIndex) { return(m_pnRootParameterIndices[nIndex]); }

	UINT GetTextureType() { return(m_nTextureType); }
	UINT GetTextureType(int nIndex) { return(m_pnResourceTypes[nIndex]); }
	DXGI_FORMAT GetBufferFormat(int nIndex) { return(m_pdxgiBufferFormats[nIndex]); }
	int GetBufferElements(int nIndex) { return(m_pnBufferElements[nIndex]); }

	D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(int nIndex);

	void ReleaseUploadBuffers();
};