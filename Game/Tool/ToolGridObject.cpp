#include "ToolGridObject.h"
#include "Shader.h"
#include "Material.h"

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
    SetBOB(fWidth, fHeight, fDepth);

    shared_ptr<CDiffusedShader> pGridShader = make_shared<CDiffusedShader>();
    pGridShader->CreateShader(L"..\\Resources\\Shader\\Shaders.hlsli", "VSDiffused", "PSDiffused", _Device, _RootSignature);
    pGridShader->CreateShaderVariables(_Device, _CommandList);

    SetShader(pGridShader);
}

ToolGridObject::~ToolGridObject()
{
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
    UpdateShaderVariables(_CommandList);

    if (m_nMaterials > 0)
    {
        for (int i = 0; i < m_nMaterials; i++)
        {
            if (m_ppMaterials[i])
            {
                if (m_ppMaterials[i]->m_pShader) m_ppMaterials[0]->m_pShader->Render(_CommandList, pCamera);
                m_ppMaterials[i]->UpdateShaderVariables(_CommandList);
            }

            for (const auto& ppMeshes : m_ppMeshes) {
                ppMeshes->RenderMesh(_CommandList, 0);
            }

        }
    }

   
}
