#include "pch.h"
#include "Camera.h"
#include "Player.h"
#include "Shader.h"

CCamera::CCamera() {
	m_xmf4x4View = Matrix4x4::Identity();
	m_xmf4x4Projection = Matrix4x4::Identity();
	m_d3dViewport={ 0, 0, Factor::FRAME_BUFFER_WIDTH, Factor::FRAME_BUFFER_HEIGHT,0.0f,1.0f };
	m_d3dScissorRect = { 0,0,Factor::FRAME_BUFFER_WIDTH,Factor::FRAME_BUFFER_HEIGHT };


}
CCamera::~CCamera() {

}


void CCamera::SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight, float fMinZ, float fMaxZ) {
	m_d3dViewport.TopLeftX = float(xTopLeft);
	m_d3dViewport.TopLeftY = float(yTopLeft);
	m_d3dViewport.Width = float(nWidth);
	m_d3dViewport.Height = float(nHeight);
	m_d3dViewport.MinDepth = fMinZ;
	m_d3dViewport.MaxDepth = fMaxZ;
}

void CCamera::SetScissorRect(LONG xLeft, LONG yTop, LONG xRight, LONG yBottom) {
	m_d3dScissorRect.left = xLeft;
	m_d3dScissorRect.top = yTop;
	m_d3dScissorRect.right = xRight;
	m_d3dScissorRect.bottom = yBottom;
}

void CCamera::GenerateProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance,
	float fAspectRatio, float fFOVAngle)
{
	m_xmf4x4Projection = Matrix4x4::PerspectiveFovLH(XMConvertToRadians(fFOVAngle),
		fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}
void CCamera::GenerateViewMatrix(XMFLOAT3 xmf3Position, XMFLOAT3 xmf3LookAt, XMFLOAT3
	xmf3Up)
{
	m_xmf4x4View = Matrix4x4::LookAtLH(xmf3Position, xmf3LookAt, xmf3Up);
}
void CCamera::CreateShaderVariables(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	UINT ncbElementBytes = ((sizeof(VS_CB_CAMERA_INFO) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbCamera = Util::CreateBufferResource(_Device.Get(), _CommandList.Get(), NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbCamera->Map(0, NULL, (void**)&m_pcbMappedCamera);
}
void CCamera::UpdateShaderVariables(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	XMStoreFloat4x4(&m_pcbMappedCamera->m_xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	XMStoreFloat4x4(&m_pcbMappedCamera->m_xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));

	D3D12_GPU_VIRTUAL_ADDRESS d3dGpuVirtualAddress = m_pd3dcbCamera->GetGPUVirtualAddress();
	_CommandList->SetGraphicsRootConstantBufferView(0, d3dGpuVirtualAddress);

	/*
	XMFLOAT4X4 xmf4x4View;
	XMFLOAT4X4 xmf4x4Projection;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4View)));
	XMStoreFloat4x4(&xmf4x4Projection, XMMatrixTranspose(XMLoadFloat4x4(&m_xmf4x4Projection)));
	_CommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4View, 0);
	_CommandList->SetGraphicsRoot32BitConstants(0, 16, &xmf4x4Projection, 16);*/

}
void CCamera::ReleaseShaderVariables()
{
	if (m_pd3dcbCamera)
	{
		m_pd3dcbCamera->Unmap(0, NULL);
		//m_pd3dcbCamera->Release();
	}
}
void CCamera::SetViewportsAndScissorRects(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	_CommandList->RSSetViewports(1, &m_d3dViewport);
	_CommandList->RSSetScissorRects(1, &m_d3dScissorRect);
}