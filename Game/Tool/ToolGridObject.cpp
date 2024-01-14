#include "ToolGridObject.h"
#include "Shader.h"

ToolGridObject::ToolGridObject(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, 
    const ComPtr<ID3D12RootSignature>& _RootSignature, int nWidth, int nLength, int nBlockWidth, int nBlockLength, XMFLOAT3 xmf3Scale, XMFLOAT4 xmf4Color)
{
    m_nWidth = nWidth;
    m_nLength = nLength;

    int cxQuadsPerBlock = nBlockWidth - 1;
    int czQuadsPerBlock = nBlockLength - 1;

    m_xmf3Scale = xmf3Scale;

    long cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
    long czBlocks = (m_nLength - 1) / czQuadsPerBlock;

    m_nMeshes = cxBlocks * czBlocks;
    m_ppMeshes.resize(cxBlocks * czBlocks, nullptr);
  
    shared_ptr<GridMesh> pGridMesh;
    for (int z = 0, zStart = 0; z < czBlocks; z++)
    {
        for (int x = 0, xStart = 0; x < cxBlocks; x++)
        {
            xStart = x * (nBlockWidth - 1);
            zStart = z * (nBlockLength - 1);
            pGridMesh = make_shared<GridMesh>(_Device, _CommandList, xStart, zStart, nBlockWidth, nBlockLength, xmf3Scale, xmf4Color);
            SetMesh(x + (z * cxBlocks), pGridMesh);
        }
    }

    float fWidth = static_cast<float>(nWidth) * xmf3Scale.x;
    float fHeight = 0.0f;
    float fDepth = static_cast<float>(nLength) * xmf3Scale.z;

    SetPosition(XMFLOAT3(-fWidth / 2, 0.0f, -fDepth / 2));


    SetOOBB(fWidth, fHeight, fDepth);

    //스마트 포인터화 필요
    CDiffusedShader* pShader = new CDiffusedShader();
    pShader->CreateShader(L"..\\Resources\\Shader\\Shaders.hlsli", "VSDiffused", "PSDiffused", _Device, _RootSignature);
    pShader->CreateShaderVariables(_Device, _CommandList);
    SetShader(pShader);
}

ToolGridObject::~ToolGridObject()
{
}

void ToolGridObject::UpdateBoundingBox()
{
    m_xmOOBB.Center = { 0.0f, 0.0f, 0.0f };
    XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
}


void ToolGridObject::SetPosition(XMFLOAT3 xmf3Position)
{
    m_xmf4x4World._41 = xmf3Position.x;
    m_xmf4x4World._42 = xmf3Position.y;
    m_xmf4x4World._43 = xmf3Position.z;
}

void ToolGridObject::SetOOBB(float fWidth, float fHeight, float fDepth)
{
    m_xmOOBB = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fWidth * 0.5f, fHeight * 0.5f, fDepth * 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
}

void ToolGridObject::SetMesh(int nIndex, const shared_ptr<CMesh> pMesh)
{
    if (nIndex >= 0 && nIndex < m_ppMeshes.size())
    {
        m_ppMeshes[nIndex] = pMesh;
    }
}

void ToolGridObject::Render(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, CCamera* pCamera)
{
    OnPrepareRender();

    if (m_pShader)
    {        
        m_pShader->UpdateShaderVariable(_CommandList, &m_xmf4x4World);
        m_pShader->Render(_CommandList, pCamera);
    }
    m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
    for (const auto& ppMeshes : m_ppMeshes) {
        ppMeshes->Render(_CommandList);
    }
}
